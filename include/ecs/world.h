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
        EntityGenerator _generator;
        std::unordered_set<Entity> _entities;
        std::unordered_map<std::type_index, std::any> _component_pools;
        std::vector<std::function<void()>> _systems;
        std::unordered_map<std::type_index, std::function<void(Entity)>> _entity_removers;
        std::unordered_map<std::type_index, std::function<void()>> _entity_clearers;

        template<typename Component>
        ComponentPool<Component>& get_pool()
        {
            std::type_index index(typeid(Component));
            if (_component_pools.find(index) == _component_pools.end())
            {
                _component_pools[index] = std::make_shared<ComponentPool<Component>>();
                _entity_removers[index] = [this](Entity e) 
                {
                    get_pool<Component>().remove(e);
                };
                _entity_clearers[index] = [this]() 
                {
                    get_pool<Component>().clear();
                };
            }
            return (std::any_cast<ComponentPool<Component>&>(_component_pools.at(index)));
        }

        template<typename Component>
        bool has_component(Entity e) 
        {
            return (get_pool<Component>().has(e));
        }

        template<typename Component>
        Component& get_component(Entity e) 
        {
            return (get_pool<Component>().get(e));
        }

    public:
        template<typename... Components>
        Entity create_entity(Components&&... comps) 
        {
            Entity entity = _generator.generate();
            _entities.insert(entity);
            (add_component<Components>(entity, std::forward<Components>(comps)), ...);
            return (entity);
        }

        Entity remove_entity(Entity entity) 
        {
            for (auto& [_, remover] : _entity_removers) 
            {
                remover(entity);
            }
            _entities.erase(entity);
            return entity;
        }

        void clear_entities() 
        {
            for (auto& [_, clearer] : _entity_clearers) 
            {
                clearer();
            }
            _entities.clear();
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
				_component_pools.erase(std::type_index(typeid(Component)));
                _entity_removers.erase(std::type_index(typeid(Component)));
                _entity_clearers.erase(std::type_index(typeid(Component)));
			}
        }

        template<typename... Components>
        void add_system(std::function<void(Entity, Components&...)> system) 
        {
            _systems.emplace_back([=, this]() 
                {
                auto& pool = get_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
                for (auto& [entity, _] : pool.all()) 
                {
                    if ((has_component<Components>(entity) && ...)) 
                    {
                        system(e, get_component<Components>(entity)...);
                    }
                }
                });
        }

        void run_systems() 
        {
            for (auto& sys : _systems) {
                sys();
            }
        }
    
    };

}