#pragma once
#include <bitset>

#include "ecs_types.h"

namespace System::ECS
{
    template<size_t MaxResource, typename... Resources>
    class Syscall
    {
    private:
        ResourceManager<MaxResource, Resources...> &_rm;
        ResourceManager<MaxResource, Resources...> _to_add_components{};
        std::tuple<decltype((void) sizeof(Resources), std::bitset<MaxResource>{})...> _to_remove_components;

        template<typename Component, std::size_t... I>
        void _remove_component_impl(pid id, std::index_sequence<I...>)
        {
            (...,
             (std::is_same_v<Component, std::tuple_element_t<I, std::tuple<Resources...>>>
                      ? (std::get<I>(_to_remove_components).set(id), void())
                      : void()));
        }

    public:
        explicit Syscall(ResourceManager<MaxResource, Resources...> &rm) : _rm(rm) {};

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
            pid id = _rm.add_process();
            (add_component(id, std::forward<Components>(components)), ...);
            return id;
        }

        void remove_entity(const pid id)
        {
            (remove_component<Resources>(id), ...);
        }

        void exec()
        {
            _rm.import(_to_add_components);
            _rm.remove_marked(_to_remove_components);

            _to_add_components.clear();
            std::apply([](auto &...bitsets) { (bitsets.reset(), ...); }, _to_remove_components);
        }
    };
} // namespace System::ECS
