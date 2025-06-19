#pragma once
#include <array>
#include <vector>
#include <deque>
#include <bitset>
#include <stdexcept>
#include <algorithm>
#include <execution>
#include <mutex>
#include <atomic>

namespace System
{
    using pid = uint64_t;

    struct RenderComponent
    {
        uint32_t x, y;
    };

    template <std::size_t MaxResource, typename Resource>
    class ResourcePool;

    template <std::size_t MaxResource, typename... Resources>
    class ResourceManager;
    template<typename Registry, auto... Task>
    class TaskManager;

    template <std::size_t MaxResource, typename Resource>
    class ResourcePool
    {
        std::vector<Resource> _data;
        std::vector<pid> _index_to_id;
        std::vector<size_t> _id_to_index;
        std::bitset<MaxResource> _has_component;

    private: 
        struct Iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<pid, Resource&>;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using reference = value_type&;

            Iterator() : _pool(nullptr), _idx(SIZE_MAX) {}
            Iterator(ResourcePool<MaxResource, Resource> *pool, size_t idx = 0): _pool(pool), _idx(idx){}

            value_type operator*()
            {
                return { _pool->_index_to_id[_idx], _pool->_data[_idx] };
            }

            Iterator& operator++()
            {
                ++_idx;
                return (*this);
            }
            bool operator==(const Iterator& other) const { return (_idx == other._idx); }
            bool operator!=(const Iterator& other) const { return !(*this == other); }

        private:
            ResourcePool<MaxResource, Resource> *_pool;
            size_t _idx = 0;
        };

    public:
        ResourcePool()
        {
            _id_to_index.resize(MaxResource, SIZE_MAX);
        }

        Resource& get(pid id)
        {
            return(_data[_id_to_index.at(id)]);
        }

        bool has(pid id) const noexcept
        {
            return(id < _id_to_index.size() && _has_component.test(id));
        }

        Iterator begin()
        {
            return(Iterator(this));
        }

        Iterator end()
        {
            return(Iterator(this, _data.size()));
        }

        void add(pid id, const Resource& value)
        {
            if (_has_component.test(id))
            {
                throw std::runtime_error("Component already exists");
            }

            size_t idx = _data.size();
            _data.push_back(value);
            _id_to_index[id] = idx;
            _index_to_id.push_back(id);
            _has_component.set(id);
        }

        void remove(pid id)
        {
            size_t index = _id_to_index.at(id);
            size_t last = _data.size() - 1;

            if (index != last)
            {
                _data[index] = std::move(_data[last]);
                pid last_id = _index_to_id[last];
                _index_to_id[index] = last_id;
                _id_to_index[last_id] = index;
            }

            _data.pop_back();
            _index_to_id.pop_back();
            _id_to_index[id] = SIZE_MAX;
            _has_component.reset(id);
        }
    };

    template <std::size_t MaxResource, typename... Resources>
    class ResourceManager
    {
    private:
        pid _id = 0;
        std::tuple<ResourcePool<MaxResource, RenderComponent>, ResourcePool<MaxResource, Resources>...> _pools;

        template <std::size_t... Index>
        auto _create_pools(std::index_sequence<Index...>)
        {
            return(std::make_tuple(ResourcePool<MaxResource, RenderComponent>(), ResourcePool<MaxResource, Resources>()...));
        }

        template<typename ResourcePool>
        void maybe_remove(ResourcePool& pool, pid id)
        {
            if (pool.has(id)) pool.remove(id);
        }

    public:
        explicit ResourceManager()
        : _pools(_create_pools(std::index_sequence_for<Resources...>{}))
        {}

        template<typename Resource>
        ResourcePool<MaxResource, Resource> &query()
        {
            return(std::get<ResourcePool<MaxResource, Resource>>(_pools));
        }

        template<typename Resource>
        const ResourcePool<MaxResource, Resource> &query() const
        {
            return(std::get<ResourcePool<MaxResource, Resource>>(_pools));
        }

        template<typename Resource>
        void add_resource(pid id, Resource &&component)
        {
            auto &pool = query<Resource>();
            pool.add(id, std::forward<Resource>(component));
        }

        void delete_entity(pid id)
        {
            std::apply(
                [&](auto&... pool)
                {
                    (maybe_remove(pool,id),...);
                },
                _pools
            );
        }

        pid add_process()
        {
            /*
             * Using circular model 
             * TODO: Make this into compact array
             *       Don't forget to update the component pool
             *       Might using dirty mark so we kept O(1) system.
             */
            _id = (_id+1)%MaxResource;
            return(_id);
        }
    };

    template<typename ResourceManager, auto... Tasks>
    class TaskManager
    {
        ResourceManager& _resource_manager;

        template<typename...>
        using void_t = void;

        template<typename Func>
        struct function_traits;

        template<typename Ret, typename Entity, typename... Args>
        struct function_traits<Ret(Entity, Args...)>
        {
            using return_type = Ret;
            using component_tuple = std::tuple<Args...>;
        };

        template<typename Ret, typename Entity, typename... Args>
        struct function_traits<Ret(*)(Entity, Args...)>
        {
            using return_type = Ret;
            using component_tuple = std::tuple<Args...>;
        };

        template<typename Func>
        struct function_traits
            : function_traits<decltype(&Func::operator())>{};

        template<typename Func, std::size_t... I>
        inline void _run_impl(Func system, std::index_sequence<I...>) const noexcept
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;

            using First = std::remove_reference_t<std::tuple_element_t<0, ComponentTuple>>;
            auto& main_pool = _resource_manager.template query<First>();

            auto cached_pools = std::tie(
                _resource_manager.template query<std::remove_reference_t<std::tuple_element_t<I, ComponentTuple>>>()...
            );

            std::for_each(std::execution::par, main_pool.begin(), main_pool.end(),
                [cached_pools, system](auto pair) {
                    auto id = pair.first;
                    if ((... && std::get<I>(cached_pools).has(id)))
                        system(id, std::get<I>(cached_pools).get(id)...);
                });
        }

        template<typename Func>
        inline void _run(Func system) const noexcept
        {
            using FnTraits = function_traits<Func>;
            using ComponentTuple = typename FnTraits::component_tuple;
            constexpr std::size_t N = std::tuple_size_v<ComponentTuple>;
            _run_impl(system, std::make_index_sequence<N>{});
        }

    public:
        explicit TaskManager(ResourceManager& reg)
        : _resource_manager(reg) {}

        inline void run_all() const
        {
            (_run(Tasks), ...);
        }
    };
}