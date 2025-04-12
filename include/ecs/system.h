#pragma once

#include "entity_manager.h"
#include "component_manager.h"

namespace ECS 
{
    class System 
    {
    public:
        virtual ~System() = default;
        virtual void update(float delta_time, ComponentManager& components, EntityManager& entities) = 0;
    };
}