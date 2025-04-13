#pragma once

#include <unordered_set>
#include <typeindex>
#include <any>

#include "memory"
#include "component_pool.h"
#include "entity_generator.h"
#include "functional"

namespace ECS
{
    class World {
    private:
        EntityGenerator generator;
        std::unordered_set<Entity> entities;
        std::unordered_map<std::type_index, std::any> component_pools;
        std::vector<std::function<void()>> systems;
        std::unordered_map<std::type_index, std::function<void(Entity)>> entity_removers;
        std::unordered_map<std::type_index, std::function<void()>> entity_clearers;

        template<typename Component>
        ComponentPool<Component>& get_pool()
        {
            std::type_index index(typeid(Component));
            if (component_pools.find(index) == component_pools.end())
            {
                component_pools[index] = std::make_shared<ComponentPool<Component>>();
                entity_removers[index] = [this](Entity e) 
                {
                    get_pool<Component>().remove(e);
                };
                entity_clearers[index] = [this]() 
                {
                    get_pool<Component>().clear();
                };
            }
            return (std::any_cast<ComponentPool<Component>&>(component_pools.at(index)));
        }

        template<typename Component>
        bool has_component(Entity e) 
        {
            return get_pool<Component>().has(e);
        }

        template<typename Component>
        Component& get_component(Entity e) 
        {
            return get_pool<Component>().get(e);
        }

    public:
        template<typename... Components>
        Entity create_entity(Components&&... comps) 
        {
            Entity entity = generator.generate();
            entities.insert(entity);
            (add_component<Components>(entity, std::forward<Components>(comps)), ...);
            return entity;
        }

        Entity remove_entity(Entity e) 
        {
            for (auto& [_, remover] : entity_removers) 
            {
                remover(e);
            }
            entities.erase(e);
            return e;
        }

        void clear_entities() 
        {
            for (auto& [_, clearer] : entity_clearers) 
            {
                clearer();
            }
            entities.clear();
        }

        template<typename Component>
        void add_component(Entity entity, Component component) 
        {
            get_pool<Component>().add(entity, component);
        }

        template<typename Component>
        void remove_component(Entity entity) 
        {
			auto& pool = get_pool<Component>();
            pool.remove(entity);
			if (pool.empty())
			{
				component_pools.erase(std::type_index(typeid(Component)));
                entity_removers.erase(std::type_index(typeid(Component)));
                entity_clearers.erase(std::type_index(typeid(Component)));
			}
        }

        template<typename... Components>
        void add_system(std::function<void(Entity, Components&...)> system) 
        {
            systems.emplace_back([=, this]() 
                {
                auto& pool = get_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
                for (auto& [e, _] : pool.all()) {
                    if ((has_component<Components>(e) && ...)) {
                        system(e, get_component<Components>(e)...);
                    }
                }
                });
        }

        void run_systems() 
        {
            for (auto& sys : systems) {
                sys();
            }
        }
    
    };

}