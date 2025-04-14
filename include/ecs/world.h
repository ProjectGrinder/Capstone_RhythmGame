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

    class World 
    {
    private:
        struct ComponentHandler
        {
            std::shared_ptr<void> pool;
            std::function<void(entity_id)> remove;
            std::function<void()> clear;
        };

        EntityGenerator _generator;
        std::unordered_set<entity_id> _entities;
        std::unordered_map<std::type_index, ComponentHandler> _component_handlers;
        std::vector<std::function<void()>> _systems;
        std::unordered_map<void*, size_t> _system_indices;

        template<ComponentType Component>
        ComponentPool<Component>& get_pool()
        {
            std::type_index index(typeid(Component));
            if (_component_handlers.find(index) == _component_handlers.end())
            {
                auto pool = std::make_shared<ComponentPool<Component>>();
                _component_handlers[index] = ComponentHandler{
                    pool,
                    [pool](entity_id e) { pool->erase(e); },
                    [pool]() { pool->clear(); }
                };
            }
            return (*std::static_pointer_cast<ComponentPool<Component>>(_component_handlers.at(index).pool));
        }

    public:
        World() = default;

        template<ComponentType... Components>
        entity_id create_entity(Components&&... comps) 
        {
            entity_id entity = _generator.generate_entity();
            _entities.insert(entity);
            (add_component<Components>(entity, std::forward<Components>(comps)), ...);
            return (entity);
        }

        bool has_entity(entity_id entity) const
        {
            return (_entities.contains(entity));
        }

        size_t entity_count() const
        {
            return _entities.size();
        }

        entity_id remove_entity(entity_id entity) 
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

        template<ComponentType Component>
        void add_component(entity_id entity, Component component) 
        {
            if (has_entity(entity) == false)
            {
                throw(std::out_of_range("Entity does not exist"));
            }
            get_pool<Component>().add(entity, component);
        }

        template<ComponentType Component>
        bool has_component(entity_id e)
        {
            if (has_entity(e) == false)
            {
                throw(std::out_of_range("Entity does not exist"));
            }
            return (get_pool<Component>().has(e));
        }

        template<ComponentType Component>
        Component& get_component(entity_id e)
        {
            if (has_entity(e) == false)
            {
                throw(std::out_of_range("Entity does not exist"));
            }
            return (get_pool<Component>().get(e));
        }

        template<ComponentType Component>
        void remove_component(entity_id entity) 
        {
            if (has_entity(entity) == false)
            {
                throw(std::out_of_range("Entity does not exist"));
            }

            auto& pool = get_pool<Component>();
            pool.erase(entity);
            if (pool.empty())
            {
                _component_handlers.erase(std::type_index(typeid(Component)));
            }
        }

        template<ComponentType... Components>
        void add_system(void (*system)(entity_id, Components&...))
        {
            void* key = reinterpret_cast<void*>(system);

            if (_system_indices.find(key) != _system_indices.end())
            {
                return;
            }

            _system_indices[key] = _systems.size();

            auto wrapper = [=, this]() 
            {
                auto& pool = get_pool<std::tuple_element_t<0, std::tuple<Components...>>>();
                for (auto& [entity, _] : pool.all()) 
                {
                    if ((has_component<Components>(entity) && ...)) 
                    {
                        system(entity, get_component<Components>(entity)...);
                    }
                }
            };

            _systems.push_back(wrapper);
        }

        template <ComponentType... Components>
        void remove_system(void (*system)(entity_id, Components&...))
        {
            void* key = reinterpret_cast<void*>(system);
            auto it = _system_indices.find(key);
            if (it == _system_indices.end()) return;

            size_t index = it->second;
            _systems.erase(_systems.begin() + index);
            _system_indices.erase(it);

            // Recalculate indices
            for (auto& [_, value] : _system_indices)
            {
                if (value > index)
                {
                    value--;
                }
            }
        }

        size_t system_count() const
        {
            return _systems.size();
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