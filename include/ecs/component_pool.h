#pragma once

#include <unordered_map>
#include <memory>

namespace ECS
{
    using Entity = uint32_t;
    
    struct ComponentBase {

    };

    template <typename Component>
    class ComponentPool
    {
    private:
        std::unordered_map<Entity, std::unique_ptr<ComponentBase>> _components;
    public:
        void add(Entity entity, const Component& component)
        {
            _components[entity] = std::make_unique<Component>(component);
        }
        void erase(Entity entity)
        {
            _components.erase(entity);
        }
        bool has(Entity entity) const
        {
            return (_components.find(entity) != _components.end());
        }
        const Component& get(Entity entity) const
        {
            return (*static_cast<Component*>(_components.at(entity)));
        }
        std::unordered_map<Entity, Component>& all() 
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