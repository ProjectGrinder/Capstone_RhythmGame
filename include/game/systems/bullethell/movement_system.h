#pragma once
#include <cmath>
#include "game/components.h"

#include "game/utils/physics_util.h"

// FIXME: Part of physics, consider separation into its own thread
using Position = Game::Physics::Position;
using Velocity = Game::Physics::Velocity;
using Rotation = Game::Physics::Rotation;
using Acceleration = Game::Physics::Acceleration;
using AngularVelocity = Game::Physics::AngularVelocity;

namespace Game::BulletHell
{
    template <typename T>
    void MovementSystem([[maybe_unused]] T &syscall, System::ECS::Query<Position, Rotation, Velocity, Acceleration>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query)
        {
            auto &pos = comps.get<Position>();
            auto &vel = comps.get<Velocity>();
            auto &acc = comps.get<Acceleration>();
            const auto &rot = comps.get<Rotation>();

            const float angle = rot.angleZ * acos(0.0f)/90.0f  ;

            pos.x += (vel.vx * cos(angle) - vel.vy * sin(angle)) * frame_time;
            pos.y += (vel.vx * sin(angle) + vel.vy * cos(angle)) * frame_time;

            if (acc.ax != 0)
            {
                vel.vx += comps.get<Acceleration>().ax * frame_time;
                if (acc.ax < 0)
                    vel.vx = vel.vx < acc.max_speed_x? acc.max_speed_x:vel.vx;
                else
                    vel.vx = vel.vx > acc.max_speed_x? acc.max_speed_x:vel.vx;
            }
            if (acc.ay != 0)
            {
                vel.vy += acc.ay * frame_time;
                if (acc.ay < 0)
                    vel.vy = vel.vy < acc.max_speed_y? acc.max_speed_y:vel.vy;
                else
                    vel.vy = vel.vy > acc.max_speed_y? acc.max_speed_y:vel.vy;
            }

        }
    }

    template <typename T>
    void RotationSystem([[maybe_unused]] T &task_manager, System::ECS::Query<Rotation, AngularVelocity>& query2)
    {
        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query2)
        {
            comps.get<Rotation>().angleZ += comps.get<AngularVelocity>().v * frame_time;
            if (comps.get<Rotation>().angleZ >= 360)
            {
                comps.get<Rotation>().angleZ -= 360;
            }
        }
    }
}