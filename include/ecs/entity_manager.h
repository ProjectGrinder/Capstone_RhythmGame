#pragma once

#pragma once

#include <queue>
#include <unordered_set>
#include <cstdint>

#include "entity.h"

namespace ECS {

    class EntityManager 
    {
    public:
        EntityManager(std::size_t);
        Entity create_entity();
        void destroy_entity(Entity entity);
        bool is_alive(Entity entity) const;
        const std::unordered_set<Entity>& get_all_entities() const;

    private:
        std::size_t max_entities;
        Entity next_entity_id;
        std::queue<Entity> free_ids;
        std::unordered_set<Entity> alive_entities;
    };
}
