#include "ecs.h"

namespace ECS
{
	void SystemManager::update(float delta_time, ComponentManager& components, EntityManager& entities)
    {
        std::for_each
        (
            systems.begin(),
            systems.end(),
            [&dt, &components, &entities](auto& system)
            {
                system->update(dt, components, entities);
            }
        );
    })
}