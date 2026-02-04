#pragma once
#include <bitset>

#include "ecs_types.h"

namespace System::ECS
{
    template<size_t MaxResource, typename... Resources>
    class SyscallResource
    {
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
            {
                pool.remove(id);
            }
        }

    public:
        explicit SyscallResource() : _pools(_create_pools(std::index_sequence_for<Resources...>{}))
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
            if (pool.has(id))
            {
                pool.remove(id);
            }
        }

        void delete_entity(pid id)
        {
            std::apply([&](auto &...pool) { (_remove_if_exists(pool, id), ...); }, _pools);
        }

        void clear()
        {
            std::apply([](auto &...pools) { (pools.clear(), ...); }, _pools);
        }
    };

    template<size_t MaxResource, typename... Resources>
    class Syscall
    {
    private:
        SyscallResource<MaxResource, Resources...> _to_add_components{};
        std::tuple<decltype((void) sizeof(Resources), std::bitset<MaxResource>{})...> _to_remove_components;
        std::bitset<MaxResource> _to_remove_entities{};

        ResourceManager<MaxResource, Resources...> &_resource_manager_ref;

        template<typename Component, std::size_t... I>
        void _remove_component_impl(pid id, std::index_sequence<I...>)
        {
            (...,
             (std::is_same_v<Component, std::tuple_element_t<I, std::tuple<Resources...>>>
                      ? (std::get<I>(_to_remove_components).set(id), void())
                      : void()));
        }

    public:
        explicit Syscall(ResourceManager<MaxResource, Resources...> &rm) : _resource_manager_ref(rm)
        {}

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
        pid create_entity(Components &&...components)
        {
            pid id = _resource_manager_ref.reserve_process();
            (add_component(id, std::forward<Components>(components)), ...);
            return (id);
        }

        void remove_entity(const pid id)
        {
            _to_remove_entities.set(id);
        }

        void exec()
        {
            _resource_manager_ref.import(_to_add_components);
            _resource_manager_ref.remove_marked(_to_remove_components, _to_remove_entities);

            _to_add_components.clear();
            _to_remove_entities.reset();
            std::apply([](auto &...bitsets) { (bitsets.reset(), ...); }, _to_remove_components);
        }
    };
} // namespace System::ECS
