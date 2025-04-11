#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <stdexcept>
#include <ranges>
#include "component_store.h"
#include "utils.h"

namespace ECS
{
    class ComponentManager 
    {
    public:
        template<typename T>
        void register_component() 
        {
            const std::type_index type = typeid(T);
            if (component_stores.contains(type)) return;

            component_stores[type] = std::make_unique<ComponentStore<T>>();
            component_types[type] = next_component_type++;
        }

        template<typename T>
        void add_component(Entity entity, T component) 
        {
            get_store<T>().insert(entity, std::move(component));
        }

        template<typename T>
        void remove_component(Entity entity) 
        {
            get_store<T>().remove(entity);
        }

        template<typename T>
        T& get_component(Entity entity) 
        {
            try
            {
                return (get_store<T>().get(entity));
            }
            catch (const std::runtime_error& e)
            {
                LOG_DEBUG("Error getting Component {}: {}", typeid(T).name(), e.what());
                return (nullptr);
            }
        }

        template<typename T>
        bool has_component(Entity entity) 
        {
            return (get_store<T>().has(entity));
        }

        template<typename... Components>
        bool with_components(const std::unordered_set<Entity>& alive_entities, const ComponentStore<Components>&... stores)
        {
            return
            (
                alive_entities | 
                std::views::filter
                (
                    [&](Entity e)
                    {
                        return (stores.has(e) && ...);
                    }
                )
            );
        }

        template<typename T>
        std::size_t get_component_type() 
        {
            return (component_types.at(typeid(T)));
        }

        void entity_destroyed(Entity entity);

    private:
        std::unordered_map<std::type_index, std::unique_ptr<IComponentStore>> component_stores;
        std::unordered_map<std::type_index, std::size_t> component_types;
        std::size_t next_component_type = 0;

        template<typename T>
        ComponentStore<T>& get_store() 
        {
            auto it = component_stores.find(typeid(T));
            if (it == component_stores.end())
            {
                throw (std::runtime_error("No ComponentStore of type " + std::string(typeid(T).name()) + " in ComponentManager!"));
            }
            return (*static_cast<ComponentStore<T>*>(it->second.get()));
        }
    };
}