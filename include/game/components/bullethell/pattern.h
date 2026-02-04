#pragma once
#include "game/utils/physics_util.h"

namespace Game::BulletHell
{
    // A single Pattern - not component ONLY usable for magnitude/angle movement
    struct MoveParam
    {
        float loopDelay;
        float speed;
        float angle;
        float accel;
        float angular_velocity;
        float max_speed;
        MoveParam() : loopDelay(0), speed(0), angle(0), accel(0), angular_velocity(0), max_speed(0) {}
        MoveParam(
                const float speed,
                const float angle,
                const float acceleration,
                const float angular_velocity,
                const float max_speed) :
            loopDelay(0),
            speed(speed),
            angle(angle),
            accel(acceleration),
            angular_velocity(angular_velocity),
            max_speed(max_speed)
        {}
        MoveParam(
                const float speed,
                const float angle,
                const float acceleration,
                const float angular_velocity,
                const float max_speed,
                const float loopDelay) :
            loopDelay(loopDelay),
            speed(speed),
            angle(angle),
            accel(acceleration),
            angular_velocity(angular_velocity),
            max_speed(max_speed)
        {}
    };

    // Component for keeping patterns
    // Warning : STDs
    struct Patterns
    {
        std::vector<std::pair<float, MoveParam>> patterns;
        void AddPattern(const float delay, const float speed, const float angle)
        {
            patterns.emplace_back(delay,MoveParam(speed, angle, UNASSIGNED,UNASSIGNED,UNASSIGNED, UNASSIGNED));
        }
        void AddPattern(const float delay, const float speed, const float angle, const float acceleration, const float angular_velocity, const float max_speed)
        {
            patterns.emplace_back(delay,MoveParam(speed, angle, acceleration, angular_velocity,max_speed, UNASSIGNED));
        }
        void AddPattern(const float delay, const float speed, const float angle, const float loopDelay)
        {
            patterns.emplace_back(delay,MoveParam(speed, angle, UNASSIGNED,UNASSIGNED,UNASSIGNED,loopDelay));
        }
        void AddPattern(const float delay, const float speed, const float angle, const float acceleration, const float angular_velocity, const float max_speed, const float loopDelay)
        {
            patterns.emplace_back(delay,MoveParam(speed, angle, acceleration, angular_velocity,max_speed,loopDelay));
        }
    };
}