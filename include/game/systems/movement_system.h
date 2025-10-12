#pragma once
#include "game/components.h"
#include "utils.h"

namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using AngularVelocity = Physics::AngularVelocity;

    template <typename T>
    void MovementSystem([[maybe_unused]] T &syscall, System::ECS::Query<Position, Velocity>& query)
    {
        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query)
        {
            comps.get<Position>().x += comps.get<Velocity>().vx * frame_time;
            comps.get<Position>().y += comps.get<Velocity>().vy * frame_time;
        }
    }

    template <typename T>
    void RotationSystem([[maybe_unused]] T &syscall, System::ECS::Query<Rotation, AngularVelocity>& query2)
    {
        constexpr auto frame_time = 1;

        for (auto &[id, comps] : query2)
        {
            comps.get<Rotation>().angle += comps.get<AngularVelocity>().v * frame_time;
            if (comps.get<Rotation>().angle >= 360)
            {
                comps.get<Rotation>().angle -= 360;
            }
            LOG_DEBUG("Angle: {}", comps.get<Rotation>().angle);
        }
    }
}