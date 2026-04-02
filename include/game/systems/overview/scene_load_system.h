#pragma once
#include "game/components.h"

// TODO : Render stuffs
namespace Game::Overview
{
    template<typename T>
    void spawn_scene_object(T &syscall, const SceneObject& scene_data)
    {
        const System::ECS::pid object = syscall.create_entity(Position(scene_data.posX,scene_data.posY), Rotation(), Physics::Scale());

        if ((scene_data.status_bit & 2) > 0)
        {
            const auto collider_data = scene_data.box_collider_data;
            syscall.add_components(object, Block(), Physics::RectangularCollider(collider_data.offsetX,collider_data.offsetY, collider_data.colX, collider_data.colY));
        }

        else if ((scene_data.status_bit & 1) > 0)
        {
            const auto collider_data = scene_data.circle_collider_data;
            syscall.add_components(object, Interactable(), Physics::CircularCollider(collider_data.offsetX,collider_data.offsetY, collider_data.colX, collider_data.colY));
        }

        if (scene_data.event_id !=  INVALID_ID)
        {
            syscall.add_components(object, EventState(scene_data.event_id));
        }

        // const Battle::GraphicData bullet_graphic = bullet_info.graphic_data;
        // syscall.add_components(bullet, Render::Sprite(bullet_graphic.sprite), Render::Material(nullptr,nullptr, 0));


    }

    template<typename T>
    void load_scene_objects([[maybe_unused]] T &syscall, System::ECS::Query<SceneRegistry> &query)
    {
        if (query.begin() == query.end())
            return;

        auto &[initialized, scene_objects] = query.front().get<SceneRegistry>();
        if (initialized) return;
        for (auto &scene_object : scene_objects)
        {
            spawn_scene_object(syscall, scene_object);
        }
        initialized = true;
    }
} // namespace Game::Overview