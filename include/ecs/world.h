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
    template <typename... Components>
    using system_function = std::function<void(ECS::Entity, Components&...)>;

    class World {
    private:
        struct ComponentHandler
        {
            std::shared_ptr<void> pool;
            std::function<void(Entity)> remove;
            std::function<void()> clear;
        };

        EntityGenerator _generator;
        std::unordered_set<Entity> _entities;
        std::unordered_map<std::type_index, ComponentHandler> _component_handlers;
        std::vector<std::function<void()>> _systems;

        template<typename Component>
        ComponentPool<Component>& get_pool()
        {
            std::type_index index(typeid(Component));
            if (_component_handlers.find(index) == _component_handlers.end())
            {
                auto pool = std::make_shared<ComponentPool<Component>>();
                _component_handlers[index] = ComponentHandler{
                    pool,
                    [pool](Entity e) { pool->erase(e); },
                    [pool]() { pool->clear(); }
                };
            }
            return (*std::static_pointer_cast<ComponentPool<Component>>(_component_handlers.at(index).pool));
        }

    public:
		World() = default;

        template<typename... Components>
        Entity create_entity(Components&&... comps) 
        {
            Entity entity = _generator.generate_entity();
            _entities.insert(entity);
            (add_component<Components>(entity, std::forward<Components>(comps)), ...);
            return (entity);
        }

        Entity remove_entity(Entity entity) 
        {
            for (auto& [_, handler] : _component_handlers)
            {
                handler.remove(entity);
            }
            _entities.erase(entity);
            return entity;
        }

        void clear_entities() 
        {
            for (auto& [_, handler] : _component_handlers) 
            {
                handler.clear();
            }
            _entities.clear();
        }

        template<typename Component>
        void add_component(Entity entity, Component component) 
        {
            get_pool<Component>().add(entity, component);
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

        template<typename Component>
        void remove_component(Entity entity) 
        {
            auto& pool = get_pool<Component>();
            pool.erase(entity);
            if (pool.empty())
            {
                _component_handlers.erase(std::type_index(typeid(Component)));
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
                        system(entity, get_component<Components>(entity)...);
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

        void reset()
        {
            
            clear_entities();
            _component_handlers.clear();
            _systems.clear();
            _generator.reset();
        }
    
    };

}