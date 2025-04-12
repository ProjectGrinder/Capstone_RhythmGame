#pragma once

#include <vector>
#include <memory>
#include "system.h"
#include "entity_manager.h"
#include "component_manager.h"
#include <algorithm>

namespace ECS 
{
    class SystemManager 
    {
    public:
        template<typename T, typename... Args>
        T& add_system(Args&&... args) 
        {
            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *system;
            systems.push_back(std::move(system));
            return (ref);
        }

        void update(float dt, ComponentManager& components, EntityManager& entities) 
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
        }

    private:
        std::vector<std::unique_ptr<System>> systems;
    };
}
