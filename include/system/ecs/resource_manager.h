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
        std::bitset<MaxResource> _dirty{};
        bool overfilled = false;

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
        void _import_pool(SyscallResource<MaxResource, Resources...> &other)
        {
            auto &target_pool = this->query<Resource>();
            auto &source_pool = other.template query<Resource>();
            for (auto it = source_pool.begin(); it != source_pool.end(); ++it)
            {
                if (auto [id, component] = *it; !target_pool.has(id))
                {
                    target_pool.add(id, component);
                    ++_component_count[id];
                    if (_dirty.test(id))
                    {
                        _dirty.reset(id);
                    }
                }
            }
        }

        template<size_t... I>
        void _import_impl(SyscallResource<MaxResource, Resources...> &other, std::index_sequence<I...>)
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

        void _rebind(pid old_id, pid new_id)
        {
            std::apply([&](auto &...pool) { (pool.rebind(old_id, new_id), ...); }, _pools);
            std::swap(_component_count[old_id], _component_count[new_id]);
        }

        pid _compact()
        {
            pid empty_id = 0;
            pid full_id = MaxResource - 1;

            while (empty_id < full_id)
            {
                // Find the next empty slot from the front
                while (empty_id < full_id && _component_count[empty_id] != 0)
                    ++empty_id;

                // Find the next full slot from the back
                while (empty_id < full_id && _component_count[full_id] == 0)
                    --full_id;

                if (empty_id >= full_id)
                    break;

                _rebind(full_id, empty_id);

                ++empty_id;
                --full_id;
            }

            // Scan for the first empty slot, starting from empty_pid
            while (empty_id < MaxResource && _component_count[empty_id] != 0)
                ++empty_id;

            return empty_id;
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

        void remove_entity(pid id)
        {
            _dirty.set(id);
            _component_count[id] = 0;
        }

        pid reserve_process()
        {
            _id++;
            if (_id == MaxResource && !overfilled)
            {
                overfilled = true;
                _id = 0;
            }
            else if (_id >= MaxResource)
            {
                throw std::runtime_error("No free pid slot available");
            }

            // if clean loop, this returns as O(1)
            if (_component_count[_id] == 0)
            {
                return (_id);
            }

            // if dirty loop, this returns as O(N)
            while (_id < MaxResource)
            {
                _id++;
                if (_component_count[_id] == 0)
                {
                    return (_id);
                }
            }

            // if no available slot after the dirty loop, throw exception
            throw std::runtime_error("No free pid slot available");
        }

        void import(SyscallResource<MaxResource, Resources...> &other)
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

            if (overfilled)
            {
                _id = _compact();
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
