#pragma once

#include <unordered_map>

namespace ECS 
{
    using Entity = uint16_t;

    class IComponentStore 
    {
    public:
        virtual ~IComponentStore();
        virtual void entity_destroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentStore : public IComponentStore 
    {
    public:
        void insert(Entity entity, T component) 
        {
            components[entity] = std::move(component);
        }

        void remove(Entity entity) 
        {
            components.erase(entity);
        }

        T& get(Entity entity) 
        {
            if (components.find(entity) == components.end()) 
            {
                throw std::runtime_error("Component not found for entity");
            }
            return (components.at(entity));
        }

        bool has(Entity entity) const 
        {
            return (components.find(entity) != components.end());
        }

        void entity_destroyed(Entity entity) override 
        {
            remove(entity);
        }

    private:
        std::unordered_map<Entity, T> components;
    };
}
