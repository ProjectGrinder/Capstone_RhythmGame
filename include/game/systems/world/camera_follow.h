#pragma once
#include "game/utils/constant.h"

namespace Game::World
{
    template<typename T>
    void camera_follow(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Render::Camera2D> &camera_query,
            System::ECS::Query<Player, Render::Transform> &player_query,
            System::ECS::Query<CameraAttractor, Render::Transform> &camera_attr_query,
            System::ECS::Query<Background, Render::Transform> &bg_query)
    {
        if (camera_query.begin() == camera_query.end())
            return;

        if (player_query.begin() == player_query.end())
            return;

        const auto &player_pos = player_query.front().components.get<Render::Transform>().position;
        auto &camera_pos = camera_query.front().get<Render::Camera2D>().offset;

        int current_order = -1;
        Math::Point camera_target = player_pos;

        for (auto &[id, comps] : camera_attr_query)
        {
            auto cam_attr = comps.get<CameraAttractor>();
            if (cam_attr.lifetime <=0)
            {
                syscall.template remove_component<CameraAttractor>(id);
            }
            if (cam_attr.order > current_order)
            {
                current_order = cam_attr.order;
                camera_target = comps.get<Render::Transform>().position;
            }
            cam_attr.lifetime-= static_cast<float>(get_delta_time());
        }

        // Cam follow
        camera_pos = Physics::lerp(camera_pos, camera_target, CAMERA_SPEED * (float)get_delta_time());
        for (auto &[id, comps] : bg_query)
        {
            const float pan = 1 - comps.get<Background>().pan_value;
            auto new_pos = Math::Point{300, 0} + player_pos * Math::Point{pan, 1};
            auto &bg_pos = comps.get<Render::Transform>().position;
            bg_pos = Physics::lerp(bg_pos, new_pos, CAMERA_SPEED * (float)get_delta_time());
        }
    }
} // namespace Game::Overview
