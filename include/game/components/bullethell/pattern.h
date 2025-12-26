#pragma once
#include "../physics/velocity.h"
#include "../physics/acceleration.h"

namespace Game::BulletHell
{
    // A single Pattern - not component
    struct MoveParam
    {
        Physics::Velocity velocity;
        float angle;
        Physics::Acceleration acceleration;
        float angular_velocity;
        MoveParam(const Physics::Velocity velocity, const float angle, const Physics::Acceleration acceleration, const float angular_velocity) :
            velocity(velocity), angle(angle), acceleration(acceleration), angular_velocity(angular_velocity)
        {}
    };

    // Component for keeping patterns
    // Warning : STDs
    struct Patterns
    {
        std::vector<std::pair<float, MoveParam>> patterns;
        void AddPattern(const float delay, const Physics::Velocity velocity, const float angle)
        {
            patterns.emplace_back(delay,MoveParam(velocity, angle, Physics::Acceleration(),0));
        }
        void AddPattern(const float delay, const Physics::Velocity velocity, const float angle, const Physics::Acceleration acceleration, const float angular_velocity)
        {
            patterns.emplace_back(delay,MoveParam(velocity, angle, acceleration, angular_velocity));
        }
    };
}