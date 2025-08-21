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
        std::bitset<MaxResource> _occupied{};
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
        void _import_pool(SyscallResource<MaxResource, Resources...> &other, pid id, bool &has_any_component)
        {
            auto &target_pool = this->query<Resource>();

            if (auto &source_pool = other.template query<Resource>(); source_pool.has(id))
            {
                has_any_component = true;
                if (target_pool.has(id))
                {
                    // UNUSED: dirty mark is not implemented in the current algorithm
                    // Case: Both source and target have the component -> Update the existing component
                    target_pool.set(id, source_pool.get(id));
                }
                else
                {
                    // Case: Source has a component, target doesn't -> Add the new component
                    target_pool.add(id, source_pool.get(id));
                    ++_component_count[id];
                }
            }
            else if (_dirty.test(id) && target_pool.has(id))
            {
                // UNUSED: dirty mark is not implemented in the current algorithm
                // Case 1: Target has a component but the source doesn't -> Remove the old component
                target_pool.remove(id);
            }
        }

        template<size_t... I>
        void _import_impl(SyscallResource<MaxResource, Resources...> &other, std::index_sequence<I...>)
        {
            for (pid id = 0; id < MaxResource; ++id)
            {
                if (_dirty.test(id))
                {
                    // UNUSED: dirty mark is not implemented in the current algorithm
                    bool has_any_component = false;
                    (_import_pool<std::tuple_element_t<I, std::tuple<Resources...>>>(other, id, has_any_component),
                     ...);

                    if (has_any_component)
                    {
                        _dirty.reset(id);
                    }
                }
                else
                {
                    // For non-dirty entities, only import if they don't exist in target
                    bool has_any_component = false;
                    (_import_pool<std::tuple_element_t<I, std::tuple<Resources...>>>(other, id, has_any_component),
                     ...);
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
                    --_component_count[id];
                }
            }
        }

        template<std::size_t... I>
        void _remove_marked_impl(const _remove_tuple_t &to_remove, std::index_sequence<I...>)
        {
            (_remove_marked_in_pool<std::tuple_element_t<I, std::tuple<Resources...>>>(std::get<I>(to_remove)), ...);
        }

        // UNUSED: compaction is not used in the current algorithm
        void _rebind(pid old_id, pid new_id)
        {
            std::apply([&](auto &...pool) { (pool.rebind(old_id, new_id), ...); }, _pools);
            std::swap(_component_count[old_id], _component_count[new_id]);
        }

        // UNUSED: compaction is not used in the current algorithm
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

        pid reserve_process()
        {
            // increment _id until finds unoccupied space
            while (_id < MaxResource)
            {
                if (_occupied.test(_id))
                {
                    _id++;
                }
                else
                {
                    _occupied.set(_id);
                    return _id++;
                }
            }

            // did not find available slot forward, mark as overfilled and retry from the start
            overfilled = true;
            _id = 0;

            while (_id < MaxResource)
            {
                if (_occupied.test(_id))
                {
                    _id++;
                }
                else
                {
                    _occupied.set(_id);
                    return _id++;
                }
            }

            // if no available slot, throw exception
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
        }

        void clear()
        {
            std::apply([](auto &...pools) { (pools.clear(), ...); }, _pools);
            _component_count.fill(0);
            _id = 0;
        }
    };
} // namespace System::ECS
