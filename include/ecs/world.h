#pragma once

#include <unordered_set>
#include <typeindex>
#include <ranges>
#include <algorithm>

#include "memory"
#include "component_pool.h"
#include "entity_generator.h"
#include "functional"
#include "system_function.h"

namespace ECS
{
    enum SystemType : bool  
    {
        UPDATE = false,  
        CLEANUP = true  
    };

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
        std::vector<std::function<void()>> _update_systems;
        std::vector<std::function<void()>> _cleanup_systems;
        std::unordered_set<void*> _systems;

        template<ComponentType Component>
        ComponentPool<Component>& get_pool()
        {
            std::type_index index(typeid(Component));
            if (!_component_handlers.contains(index))
            {
                auto pool = std::make_shared<ComponentPool<Component>>();
                _component_handlers[index] = ComponentHandler
                {
                    pool,
                    [pool](entity_id e) 
                    { 
                        pool->erase(e); 
                    },
                    [pool]() 
                    { 
                        pool->clear(); 
                    }
                };
            }
            return (*std::static_pointer_cast<ComponentPool<Component>>(_component_handlers.at(index).pool));
        }

        void cleanup()
        {
            for (auto sys : _cleanup_systems)
            {
                sys();
            }
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
            return (_entities.size());
        }

        entity_id remove_entity(entity_id entity) 
        {
            for (auto& [_, handler] : _component_handlers)
            {
                handler.remove(entity);
            }
            _entities.erase(entity);
            return (entity);
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
        void add_system(SystemFunction<Components...> system, SystemType type)
        {
            void* key = reinterpret_cast<void*>(system);
        
            if (_systems.contains(key))
            {
                return;
            }
        
            _systems.insert(key);
        
            auto wrapper = [this, system]()
            {
                ECS::EntityMap<Components...> matching_entities;

                auto filter = [this](entity_id entity)
                {
                    return(has_component<Components>(entity) && ...);
                };

                auto apply = [this, system, &matching_entities](entity_id entity)
                {
                    matching_entities.emplace(
                        std::piecewise_construct,
                        std::forward_as_tuple(entity),
                        std::forward_as_tuple(get_component<Components>(entity)...)
                    );
                };

                std::ranges::for_each
                (
                    _entities | std::views::filter(filter),
                    apply
                );

                system(matching_entities);
            };

            switch (type)
            {
            case CLEANUP:
                _cleanup_systems.push_back(wrapper);
                break;
            case UPDATE:
                _update_systems.push_back(wrapper);
                break;
            default:
                throw std::invalid_argument("Invalid system type");
            }
        }

        size_t system_count() const
        {
            return (_update_systems.size());
        }

        void run_systems() 
        {
            for (auto& sys : _update_systems) 
            {
                sys();
            }
        }

        void reset()
        {
            cleanup();
            clear_entities();
            _component_handlers.clear();
            _update_systems.clear();
            _cleanup_systems.clear();
            _systems.clear();
            _generator.reset();
        }
    
    };

}