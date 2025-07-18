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

        template<std::size_t... Index>
        auto _create_pools(std::index_sequence<Index...>)
        {
            return (std::make_tuple(ResourcePool<MaxResource, Resources>()...));
        }

        template<typename ResourcePool>
        static void _remove_if_exists(ResourcePool &pool, pid id)
        {
            if (pool.has(id))
                pool.remove(id);
        }

        template<typename PoolType>
        void _import_pool(ResourceManager &other)
        {
            using Resource = typename PoolType::resource_type;
            auto &target_pool = this->query<Resource>();
            auto &source_pool = other.query<Resource>();
            for (auto it = source_pool.begin(); it != source_pool.end(); ++it)
            {
                if (auto [id, component] = *it; !target_pool.has(id))
                {
                    target_pool.add(id, component);
                }
            }
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
        }

        template<typename Resource>
        void remove_resource(pid id)
        {
            auto &pool = query<Resource>();
            _remove_if_exists(pool, id);
        }

        void delete_entity(pid id)
        {
            std::apply([&](auto &...pool) { (_remove_if_exists(pool, id), ...); }, _pools);
        }

        pid add_process()
        {
            /*
             * Using circular model
             * TODO: Make this into compact array
             *       Don't forget to update the component pool
             *       Might using dirty mark so we kept O(1) system.
             */
            _id = (_id + 1) % MaxResource;
            return (_id);
        }

        void import(ResourceManager &other)
        {
            std::apply(
                    [this, &other]<typename... PoolTypes>([[maybe_unused]] PoolTypes &...pools)
                    { (_import_pool<std::remove_reference_t<PoolTypes>>(other), ...); },
                    _pools);
        }


        void remove_marked(const _remove_tuple_t &to_remove)
        {
            _remove_marked_impl(to_remove, std::make_index_sequence<sizeof...(Resources)>{});
        }

        void clear()
        {
            std::apply([](auto &...pools) { (pools.clear(), ...); }, _pools);
            _id = 0;
        }
    };
} // namespace System::ECS
