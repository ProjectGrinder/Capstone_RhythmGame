#pragma once

#include <cstdint>
#include <type_traits>
#include <functional>

namespace ECS
{
    using Entity = std::uint32_t;

    // Forward declaration of EntityView
    template<typename... Components>
    class EntityView;

    // --- Entity management ---

    template<typename... Components>
    Entity create_entity();

    Entity remove_entity(Entity entity);

    bool has_entity(Entity entity);

    void clear_entities();

    // --- Component management ---

    template<typename Component>
    void add_component(Entity entity);

    template<typename Component>
    void add_component(Entity entity, const Component& component);

    template<typename Component>
    Component& get_component(Entity entity);

    template<typename Component>
    bool has_component(Entity entity);

    template<typename Component>
    void remove_component(Entity entity);

    void clear_components();

    // --- System management ---

    template<typename... Components>
    using SystemFunction = std::function<void(EntityView<Components...>&)>;

    template<typename... Components>
    void add_system(SystemFunction<Components...> system);

    template<typename... Components>
    void remove_system(SystemFunction<Components...> system);

    template<typename... Components>
    bool has_system(SystemFunction<Components...> system);

    void clear_systems();

    void reset();
}
