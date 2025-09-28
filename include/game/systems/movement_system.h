#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void MovementSystem([[maybe_unused]] T &syscall, System::ECS::Query<Position, Velocity>& query)
    {
        for (auto &[id, comps] : query)
        {
            comps.get<Position>().x += comps.get<Velocity>().vx;
            comps.get<Position>().y += comps.get<Velocity>().vy;
        }
    }

    template <typename T>
    void RotationSystem([[maybe_unused]] T &syscall, System::ECS::Query<Rotation, AngularVelocity>& query2)
    {
        for (auto &[id, comps] : query2)
        {
            comps.get<Rotation>().angle += comps.get<AngularVelocity>().v;
            if (comps.get<Rotation>().angle > 360)
            {
                comps.get<Rotation>().angle -= 360;
            }
        }
    }
}