#pragma once
#include "game/components.h"

// TODO : Render stuffs
namespace Game::World
{
    template<typename T>
    void spawn_scene_object(T &syscall, const SceneObject& scene_data)
    {
        const System::ECS::pid object = syscall.create_entity(Render::Transform(scene_data.posX,scene_data.posY, 0, 0, 0, scene_data.size_x, scene_data.size_y,1));

        const GraphicData &scene_graphic = scene_data.graphic_data;
        const auto asset_ptr = get_assets_record_ptr(get_assets_id(graphic_type_string[scene_graphic.type]));
        const Math::Point asset_size = {
                (float) asset_ptr->info.info.as_sprite.width, (float) asset_ptr->info.info.as_sprite.height};
        int src_rect[4] = {scene_graphic.src_rect[0],scene_graphic.src_rect[1],scene_graphic.src_rect[2],scene_graphic.src_rect[3]};
        float dest_rect[4] = {scene_graphic.dest_rect[0],scene_graphic.dest_rect[1],scene_graphic.dest_rect[2],scene_graphic.dest_rect[3]};
        if (src_rect[0]==0 && src_rect[1]==0 && src_rect[2]==0 && src_rect[3]==0)
        {
            src_rect[2] = (int)asset_size.x;
            src_rect[3] = (int)asset_size.y;
            dest_rect[0] = static_cast<float>(src_rect[0] - src_rect[2]);
            dest_rect[1] = static_cast<float>(src_rect[1] - src_rect[3]);
            dest_rect[2] = static_cast<float>(src_rect[2] - src_rect[0]);
            dest_rect[3] = static_cast<float>(src_rect[3] - src_rect[1]);
        }

        syscall.add_components(object, Render::Sprite{
                    .sp = asset_ptr,
                    .pos = {{dest_rect[0], dest_rect[3], 0}, {dest_rect[2], dest_rect[3], 0}, {dest_rect[2], dest_rect[1], 0}, {dest_rect[0], dest_rect[1], 0}},
                    .layer = scene_graphic.layer,
                    .u0 = static_cast<float>(src_rect[0])/asset_size.x,
                    .v0 = static_cast<float>(src_rect[1])/asset_size.y,
                    .u1 = static_cast<float>(src_rect[2])/asset_size.x,
                    .v1 = static_cast<float>(src_rect[3])/asset_size.y}
                    , Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")),get_assets_record_ptr(get_assets_id("sprite_ps")), {scene_graphic.r, scene_graphic.g, scene_graphic.b, scene_graphic.a}));

        if ((scene_data.status_bit & 2) > 0)
        {
            auto collider_data = scene_data.box_collider_data;
            if (collider_data.colX == 1 && collider_data.colY == 1)
            {
                collider_data.colX = static_cast<float>(src_rect[2] - src_rect[0]);
                collider_data.colY = static_cast<float>(src_rect[3] - src_rect[1]);
            }
            syscall.add_components(object, Block(), Physics::RectangularCollider(collider_data.offsetX,collider_data.offsetY, collider_data.colX, collider_data.colY));
        }

        else if ((scene_data.status_bit & 1) > 0)
        {
            auto collider_data = scene_data.circle_collider_data;
            if (collider_data.colX == 1 && collider_data.colY == 1)
            {
                collider_data.colX = static_cast<float>(src_rect[2] - src_rect[0]);
                collider_data.colY = static_cast<float>(src_rect[3] - src_rect[1]);
            }
            syscall.add_components(object, Interactable(), Physics::CircularCollider(collider_data.offsetX,collider_data.offsetY, collider_data.colX, collider_data.colY));
        }

        if (scene_data.event_id !=  INVALID_ID)
        {
            syscall.add_components(object, EventState(scene_data.event_id));
        }
    }

    template<typename T>
    void load_scene_objects([[maybe_unused]] T &syscall, System::ECS::Query<SceneRegistry> &query, System::ECS::Query<SaveState> &save_query)
    {
        if (query.begin() == query.end())
            return;

        if (save_query.begin() == save_query.end()) return;

        auto &[initialized, scene_objects] = query.front().get<SceneRegistry>();
        if (initialized) return;
        // const auto &save_state = save_query.front().get<SaveState>();
        for (auto &scene_object : scene_objects)
        {
            spawn_scene_object(syscall, scene_object);
        }
        initialized = true;
    }
} // namespace Game::Overview