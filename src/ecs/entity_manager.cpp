#include "ecs.h"

ECS::EntityManager::EntityManager(std::size_t max_entities)
	: max_entities(max_entities), next_entity_id(0) {}

ECS::Entity ECS::EntityManager::create_entity()
{
	Entity id;
	if (!free_ids.empty()) {
		id = free_ids.front();
		free_ids.pop();
	}
	else {
		if (next_entity_id >= max_entities) {
			throw std::runtime_error("Too many entities in use.");
		}
		id = next_entity_id++;
	}
	alive_entities.insert(id);
	return (id);
}

void ECS::EntityManager::destroy_entity(Entity entity)
{
	if (is_alive(entity)) {
		free_ids.push(entity);
		alive_entities.erase(entity);
	}
}

bool ECS::EntityManager::is_alive(Entity entity) const
{
	return (alive_entities.find(entity) != alive_entities.end());
}

const std::unordered_set<ECS::Entity>& ECS::EntityManager::get_all_entities() const
{
	return (alive_entities);
}