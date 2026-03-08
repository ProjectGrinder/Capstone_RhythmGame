#pragma once
#include <cmath>
#include "game/components.h"

#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using Acceleration = Physics::Acceleration;
    using AngularVelocity = Physics::AngularVelocity;

    template <typename T>
    void movement_system([[maybe_unused]] T &syscall, System::ECS::Query<Position, Rotation, Velocity>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps]: query)
        {
            auto &pos = comps.get<Position>();
            auto &vel = comps.get<Velocity>();
            const auto &rot = comps.get<Rotation>();

            const float angle = rot.angleZ * acos(0.0f)/90.0f  ;

            pos.x += (vel.vx * cos(angle) - vel.vy * sin(angle)) * frame_time;
            pos.y += (vel.vx * sin(angle) + vel.vy * cos(angle)) * frame_time;
        }
    }

    template <typename T>
    void AccelerationSystem([[maybe_unused]] T &syscall, System::ECS::Query<Velocity,Acceleration>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query)
        {
            auto &vel = comps.get<Velocity>();
            const auto &acc = comps.get<Acceleration>();

            vel.vx += acc.ax * frame_time;
            vel.vx = Physics::clamp(vel.vx, acc.min_speed_x, acc.max_speed_x);

            vel.vy += acc.ay * frame_time;
            vel.vy = Physics::clamp(vel.vy, acc.min_speed_y, acc.max_speed_y);

        }
    }

    template<typename T>
    void rotation_system([[maybe_unused]] T &task_manager, System::ECS::Query<Rotation, AngularVelocity> &query2)
    {
        constexpr auto frame_time = 1;

        for (auto &[id, comps]: query2)
        {
            comps.get<Rotation>().angleZ += comps.get<AngularVelocity>().v * frame_time;
            if (comps.get<Rotation>().angleZ >= 360)
            {
                comps.get<Rotation>().angleZ -= 360;
            }
        }
    }
} // namespace Game::BulletHell
