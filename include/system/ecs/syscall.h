#pragma once

#include "ecs_types.h"

namespace System::ECS
{
    template<typename T, typename... Ts>
    struct get_index;

    template<typename T, typename... Ts>
    struct get_index<T, T, Ts...> : std::integral_constant<std::size_t, 0>
    {};

    template<typename T, typename U, typename... Ts>
    struct get_index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + get_index<T, Ts...>::value>
    {};

    template<typename T, typename... Ts>
    static constexpr std::size_t type_index_v = get_index<T, Ts...>::value;

    template<size_t MaxResource, typename... Resources>
    class SyscallResource
    {
        std::tuple<ResourcePool<MaxResource, Resources>...> _pools;

    public:
        template<typename T>
        auto &query()
        {
            return std::get<ResourcePool<MaxResource, T>>(_pools);
        }

        template<typename T>
        void add(pid id, T &&component)
        {
            query<T>().add(id, std::forward<T>(component));
        }

        void clear()
        {
            std::apply([](auto &...pools) { (pools.clear(), ...); }, _pools);
        }

        auto &get_pools()
        {
            return _pools;
        }
    };

    template<size_t MaxResource, typename... Resources>
    class Syscall
    {
    private:
        SyscallResource<MaxResource, Resources...> _to_add_components;
        template<typename T>
        using BitsetAlias = std::bitset<MaxResource>;
        std::tuple<BitsetAlias<Resources>...> _to_remove_components;
        std::bitset<MaxResource> _to_remove_entities;
        std::vector<pid> _dirty_ids;

        ResourceManager<MaxResource, Resources...> &_rm;

        template<typename T>
        static constexpr size_t type_idx()
        {
            return type_index_v<std::remove_cvref_t<T>, Resources...>;
        }

        void _mark_dirty(pid id)
        {
            bool already_tracked = _to_remove_entities.test(id);

            if (!already_tracked)
            {
                std::apply([&](auto &...bits) { ((already_tracked |= bits.test(id)), ...); }, _to_remove_components);
            }

            if (!already_tracked)
            {
                _dirty_ids.push_back(id);
            }
        }

    public:
        explicit Syscall(ResourceManager<MaxResource, Resources...> &rm) : _rm(rm)
        {}

        template<typename Component>
        void add_component(pid id, Component &&component)
        {
            using T = std::remove_cvref_t<Component>;
            std::get<type_idx<T>()>(_to_remove_components).reset(id);
            _to_add_components.template add<T>(id, std::forward<Component>(component));
        }

        template<typename ...Components>
        void add_components(pid id, Components &&...components)
        {
            (add_component(id, std::forward<Components>(components)), ...);
        }

        template<typename Component>
        void remove_component(pid id)
        {
            std::get<type_idx<Component>()>(_to_remove_components).set(id);
        }

        template<typename... Components>
        pid create_entity(Components &&...components)
        {
            pid reserved_id = _rm.reserve_process();
            (add_component(reserved_id, std::forward<Components>(components)), ...);
            return reserved_id;
        }

        void remove_entity(pid id)
        {
            if (!_to_remove_entities.test(id))
            {
                _mark_dirty(id);
                _to_remove_entities.set(id);
            }
        }

        void exec()
        {
            _rm.import(_to_add_components);
            _rm.remove_marked(_to_remove_components, _to_remove_entities, _dirty_ids);

            _to_add_components.clear();
            _to_remove_entities.reset();
            std::apply([](auto &...bits) { (bits.reset(), ...); }, _to_remove_components);
        }
    };
} // namespace System::ECS
