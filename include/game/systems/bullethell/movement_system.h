#pragma once
#include <cmath>
#include "game/components.h"

#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
using Velocity = Game::Physics::Velocity;
using Rotation = Game::Physics::Rotation;
using Acceleration = Game::Physics::Acceleration;
using AngularVelocity = Game::Physics::AngularVelocity;
namespace Game::BulletHell
{
    template <typename T>
    void movement_system([[maybe_unused]] T &syscall, System::ECS::Query<Render::Transform, Rotation, Velocity>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps]: query)
        {
            auto &pos = comps.get<Render::Transform>().position;
            auto &vel = comps.get<Velocity>();
            const auto &rot = comps.get<Rotation>();

            const float angle = rot.angleZ * acos(0.0f)/90.0f  ;

            pos.x += static_cast<float>((vel.vx * cos(angle) - vel.vy * sin(angle)) * get_delta_time()/1000);
            pos.y += static_cast<float>((vel.vx * sin(angle) + vel.vy * cos(angle)) * get_delta_time()/1000);
        }
    }

    template <typename T>
    void acceleration_system([[maybe_unused]] T &syscall, System::ECS::Query<Velocity,Acceleration>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        for (auto &[id, comps] : query)
        {
            auto &vel = comps.get<Velocity>();
            const auto &acc = comps.get<Acceleration>();

            vel.vx += acc.ax * static_cast<float>(get_delta_time()/1000);
            vel.vx = Physics::clamp(vel.vx, acc.min_speed_x, acc.max_speed_x);

            vel.vy += acc.ay * static_cast<float>(get_delta_time()/1000);
            vel.vy = Physics::clamp(vel.vy, acc.min_speed_y, acc.max_speed_y);

        }
    }

    template<typename T>
    void rotation_system([[maybe_unused]] T &task_manager, System::ECS::Query<Rotation, Render::Transform, AngularVelocity> &query2)
    {
        for (auto &[id, comps]: query2)
        {
            auto &rot = comps.get<Rotation>();
            auto &tra = comps.get<Render::Transform>();
            rot.angleZ += comps.get<AngularVelocity>().v * static_cast<float>(get_delta_time()/1000);
            if (rot.angleZ >= 360)
            {
                rot.angleZ -= 360;
            }

            if (rot.attach_render)
            {
                tra.angleX = rot.angleX * acos(0.0f)/90.0f;
                tra.angleY = rot.angleY * acos(0.0f)/90.0f;
                tra.angleZ = (rot.angleZ-90) * acos(0.0f)/90.0f;
            }
        }
    }
} // namespace Game::BulletHell
