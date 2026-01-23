#pragma once
#include "game/components.h"
#include "game/components/physics/acceleration.h"
#include <cmath>

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using Acceleration = Physics::Acceleration;
    using AngularVelocity = Physics::AngularVelocity;

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
            const float angle = comps.get<Rotation>().angleZ * std::acos(0.0f)/90.0f  ;
            comps.get<Position>().x += (comps.get<Velocity>().vx * cos(angle) - comps.get<Velocity>().vy * sin(angle)) * frame_time;
            comps.get<Position>().y += (comps.get<Velocity>().vx * sin(angle) + comps.get<Velocity>().vy * cos(angle)) * frame_time;

            if (comps.get<Acceleration>().ax != 0)
            {
                comps.get<Velocity>().vx += comps.get<Acceleration>().ax * frame_time;
                comps.get<Velocity>().vx = comps.get<Velocity>().vx > comps.get<Acceleration>().max_speed? comps.get<Acceleration>().max_speed:comps.get<Velocity>().vx;
            }
            if (comps.get<Acceleration>().ay != 0)
            {
                comps.get<Velocity>().vy += comps.get<Acceleration>().ay * frame_time;
                comps.get<Velocity>().vy = comps.get<Velocity>().vy > comps.get<Acceleration>().max_speed? comps.get<Acceleration>().max_speed:comps.get<Velocity>().vy;
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

    template <typename T>
    void SyncRotationSystem([[maybe_unused]] T &task_manager, System::ECS::Query<Physics::Rotation, Render::Rotation, AngularVelocity>& query2)
    {
        for (auto &[id, comps] : query2)
        {
            comps.get<Render::Rotation>().angleZ = comps.get<Physics::Rotation>().angleZ;
        }
    }
}