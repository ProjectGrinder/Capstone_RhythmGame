#pragma once

#include "ecs/component_pool.h"
#include "ecs/entity_generator.h"
#include "ecs/world.h"

namespace ECS
{
    template <typename... Components>
    Entity create_entity(Components&&... comps)
    {
		return World::instance().create_entity(std::forward<Components>(comps)...);
    }

	Entity remove_entity(Entity entity)
	{
		return World::instance().remove_entity(entity);
	}

	void clear_entities()
	{
		return World::instance().clear_entities();
	}

	template <typename Component>
	void add_component(Entity entity, Component component)
	{
		return World::instance().add_component(entity, component);
	}

	template <typename Component>
	void remove_component(Entity entity)
	{
		return World::instance().remove_component<Component>(entity);
	}

	template <typename Component>
	bool has_component(Entity entity)
	{
		return World::instance().has_component<Component>(entity);
	}

	template <typename Component>
	Component& get_component(Entity entity)
	{
		return World::instance().get_component<Component>(entity);
	}

	template <typename... Components>
	void add_system(std::function<void(Entity, Components&...)> system)
	{
		return World::instance().add_system<Components...>(system);
	}

	void run_systems()
	{
		return World::instance().run_systems();
	}
}
