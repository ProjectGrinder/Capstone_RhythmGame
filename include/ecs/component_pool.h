#pragma once

#include <unordered_map>

namespace ECS
{
    using Entity = uint32_t;

    template <typename Component>
    class ComponentPool
    {
    private:
        std::unordered_map<Entity, Component> components;
    public:
        void add(Entity entity, const Component& component)
        {
            components[entity] = component;
        }
        void remove(Entity entity)
        {
            components.erase(entity);
        }
        bool has(Entity entity) const
        {
            return (components.find(entity) != components.end());
        }
        const Component& get(Entity entity) const
        {
            return (components.at(entity));
        }
        std::unordered_map<Entity, Component>& all() 
        { 
            return (components);
        }
        bool empty() const 
        { 
            return (components.empty());
        }
        void clear()
        {
            components.clear();
        }
    };
}