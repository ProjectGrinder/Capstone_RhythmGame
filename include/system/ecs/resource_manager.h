#pragma once
#include <array>
#include <bitset>
#include <execution>
#include <ranges>
#include "ecs_types.h"

namespace System::ECS
{
    template<std::size_t MaxResource, typename... Resources>
    class ResourceManager
    {
    private:
        pid _id = 0;
        std::tuple<ResourcePool<MaxResource, Resources>...> _pools;
        std::array<std::size_t, MaxResource> _component_count{};

        template<std::size_t... Index>
        auto _create_pools(std::index_sequence<Index...>)
        {
            return (std::make_tuple(ResourcePool<MaxResource, Resources>()...));
        }

        template<typename ResourcePool>
        static void _remove_if_exists(ResourcePool &pool, pid id)
        {
            if (pool.has(id))
            {
                pool.remove(id);
            }
        }

        template<typename Resource>
        void _import_pool(ResourceManager &other)
        {
            auto &target_pool = this->query<Resource>();
            auto &source_pool = other.query<Resource>();
            for (auto it = source_pool.begin(); it != source_pool.end(); ++it)
            {
                if (auto [id, component] = *it; !target_pool.has(id))
                {
                    target_pool.add(id, component);
                    ++_component_count[id];
                }
            }
        }

        template <size_t... I>
        void _import_impl(ResourceManager &other, std::index_sequence<I...>)
        {
            (_import_pool<std::tuple_element_t<I, std::tuple<Resources...>>>(other), ...);
        }

        using _remove_tuple_t = std::tuple<decltype((void) sizeof(Resources), std::bitset<MaxResource>{})...>;

        template<typename Resource>
        void _remove_marked_in_pool(const std::bitset<MaxResource> &bitset)
        {
            auto &pool = this->query<Resource>();
            for (size_t id = 0; id < MaxResource; ++id)
            {
                if (bitset.test(id) && pool.has(id))
                {
                    pool.remove(id);
                    --_component_count[id];
                }
            }
        }

        template<std::size_t... I>
        void _remove_marked_impl(const _remove_tuple_t &to_remove, std::index_sequence<I...>)
        {
            (_remove_marked_in_pool<std::tuple_element_t<I, std::tuple<Resources...>>>(std::get<I>(to_remove)), ...);
        }

    public:
        explicit ResourceManager() : _pools(_create_pools(std::index_sequence_for<Resources...>{}))
        {}

        template<typename Resource>
        ResourcePool<MaxResource, Resource> &query()
        {
            return (std::get<ResourcePool<MaxResource, Resource>>(_pools));
        }

        template<typename Resource>
        const ResourcePool<MaxResource, Resource> &query() const
        {
            return (std::get<ResourcePool<MaxResource, Resource>>(_pools));
        }

        template<typename Resource>
        void add_resource(pid id, Resource &&component)
        {
            auto &pool = query<Resource>();
            pool.add(id, std::forward<Resource>(component));
            ++_component_count[id];
        }

        template<typename Resource>
        void remove_resource(pid id)
        {
            if (auto &pool = query<Resource>(); pool.has(id))
            {
                pool.remove(id);
                --_component_count[id];
            }
        }

        void delete_entity(pid id)
        {
            std::apply([&](auto &...pool) { (_remove_if_exists(pool, id), ...); }, _pools);
            _component_count[id] = 0;
        }

        pid add_process()
        {
            /*
             * Using circular model
             * TODO: Make this into compact array
             *       Don't forget to update the component pool
             *       Might using dirty mark so we kept O(1) system.
             */
            const pid _old_id = _id;
            while (_component_count[_id] != 0)
            {
                _id = (_id + 1) % MaxResource;
                if (_id == _old_id)
                {
                    throw std::runtime_error("No free slot");
                }
            }
            return (_id);
        }

        void import(ResourceManager &other)
        {
            _import_impl(other, std::make_index_sequence<sizeof...(Resources)>{});
        }

        void remove_marked(const _remove_tuple_t &to_remove, const std::bitset<MaxResource> &to_remove_entities)
        {
            _remove_marked_impl(to_remove, std::make_index_sequence<sizeof...(Resources)>{});

            for (size_t id = 0; id < MaxResource; ++id)
            {
                if (to_remove_entities.test(id))
                {
                    delete_entity(id);
                }
            }
        }

        void clear()
        {
            std::apply([](auto &...pools) { (pools.clear(), ...); }, _pools);
            _component_count.fill(0);
            _id = 0;
        }
    };
} // namespace System::ECS
