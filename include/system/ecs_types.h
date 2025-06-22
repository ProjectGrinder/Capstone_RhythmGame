#pragma once
#include <array>
#include <vector>
#include <tuple>
#include <bitset>
#include <stdexcept>
#include <algorithm>
#include <execution>
#include <ranges>

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
    template<size_t MaxResource, typename... Resources>
    class Syscall;

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

        void clear() 
        {
            _data.clear();
            _index_to_id.clear();
            _id_to_index.assign(MaxResource, SIZE_MAX);
            _has_component.reset();
        }

        // type hints bless thee
        using resource_type = Resource;
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

        using _remove_tuple_t = std::tuple<decltype((void)sizeof(Resources), std::bitset<MaxResource>{})...>;

        template <std::size_t... I>
        void _remove_marked_impl(const _remove_tuple_t& to_remove, std::index_sequence<I...>) {
            (
                [&] {
                    using Resource = std::tuple_element_t<I, std::tuple<Resources...>>;
                    auto& pool = this->template query<Resource>();
                    const auto& bitset = std::get<I>(to_remove);
                    for (size_t id = 0; id < MaxResource; ++id) {
                        if (bitset.test(id) && pool.has(id)) {
                            pool.remove(id);
                        }
                    }
                }(),
                ...
            );
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

        template<typename Resource>
        void remove_resource(pid id)
        {
            auto &pool = query<Resource>();
            maybe_remove(pool, id);
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

        // Import matching components from another ResourceManager
        void import(ResourceManager& other)
        {
            std::apply(
            [this, &other](auto&... pools)
            {
                (
                    [&] 
                    {
                        using PoolType = std::remove_reference_t<decltype(pools)>;
                        using Resource = typename PoolType::resource_type;
                        auto& target_pool = this->query<Resource>();
                        auto& source_pool = other.query<Resource>();
                        for (auto it = source_pool.begin(); it != source_pool.end(); ++it)
                        {
                            if (auto [id, component] = *it; !target_pool.has(id))
                            {
                                target_pool.add(id, component);
                            }
                        }
                    }(),
                    ...
                );
            }, _pools);
        }

        void remove_marked(const _remove_tuple_t& to_remove) {
            _remove_marked_impl(to_remove, std::make_index_sequence<sizeof...(Resources)>{});
        }

        void clear() 
        {
            std::apply([](auto&... pools) { (pools.clear(), ...); }, _pools);
            _id = 0;
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

    template<size_t MaxResource, typename... Resources>
    class Syscall
    {
    private:

        ResourceManager<MaxResource, Resources...> _to_add_components{};
        std::tuple<decltype((void)sizeof(Resources), std::bitset<MaxResource>{})...> _to_remove_components;

        template<typename Component, std::size_t... I>
        void _remove_component_impl(pid id, std::index_sequence<I...>)
        {
            (..., (std::is_same_v<Component, std::tuple_element_t<I, std::tuple<Resources...>>>
                ? (std::get<I>(_to_remove_components).set(id), void())
                : void()));
        }

    public:
        Syscall() = default;

        template<typename Component>
        void add_component(pid id, Component &&component)
        {
            _to_add_components.add_resource(id, std::forward<Component>(component));
        }

        template<typename Component>
        void remove_component(pid id) 
        {
            _remove_component_impl<Component>(id, std::make_index_sequence<sizeof...(Resources)>{});
        }

        template<typename... Components>
        pid create_entity(ResourceManager<MaxResource, Resources...> &rm, Components&&... components)
        {
            pid id = rm.add_process();
            (add_component(id, std::forward<Components>(components)), ...);
            return id;
        }

        void remove_entity(pid id)
        {
            (remove_component<Resources>(id), ...);
        }

        void exec(ResourceManager<MaxResource, Resources...>& rm)
        {
            rm.import(_to_add_components);
            rm.remove_marked(_to_remove_components);
            // Clear _to_add_components and reset _to_remove_components
            _to_add_components.clear();
            std::apply([](auto&... bitsets) { (bitsets.reset(), ...); }, _to_remove_components);
        }

    };
}