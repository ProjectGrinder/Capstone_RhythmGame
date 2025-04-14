#pragma once

#include <unordered_map>
#include <memory>
#include <concepts>

namespace ECS
{
    using Entity = uint32_t;
    
    struct ComponentBase {

    };

    template<typename T>
    concept ComponentType = std::derived_from<T, ComponentBase>;

    template <ComponentType Component>
    class ComponentPool
    {
    private:
        static_assert(std::derived_from<Component, ComponentBase>, "Component must inherit from ComponentBase");
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
        Component& get(Entity entity) const
        {
            return (*static_cast<Component*>(_components.at(entity).get()));
        }
        std::unordered_map<Entity, std::unique_ptr<ComponentBase>>& all()
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