#pragma once

#include <unordered_map>
#include <memory>
#include "ecs/component_base.h"

namespace ECS
{
    using entity_id = uint32_t;

    template <ComponentType Component>
    class ComponentPool
    {
    private:
        std::unordered_map<entity_id, std::unique_ptr<ComponentBase>> _components;
    public:
        void add(entity_id entity, const Component& component)
        {
            _components[entity] = std::make_unique<Component>(component);
        }
        void erase(entity_id entity)
        {
            _components.erase(entity);
        }
        bool has(entity_id entity) const
        {
            return (_components.contains(entity));
        }
        Component& get(entity_id entity) const
        {
            return (*static_cast<Component*>(_components.at(entity).get()));
        }
        std::unordered_map<entity_id, std::unique_ptr<ComponentBase>>& all()
        { 
            return (_components);
        }
        bool empty() const 
        { 
            return (_components.empty());
        }
        void clear()
        {
            _components.clear();
        }
    };
}