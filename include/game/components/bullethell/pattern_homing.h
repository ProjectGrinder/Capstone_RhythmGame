#pragma once
#include "game/components/physics/position.h"

namespace Game::BulletHell
{
    struct Homing
    {
        bool targetPlayer;
        Physics::Position targetPosition;
        float delay, expire_time;
        float strength;
        explicit Homing(const float expire_time) : targetPlayer(true), delay(0), expire_time(expire_time), strength(1)
        {}
        explicit Homing(const float delay, const float expire_time) :
            targetPlayer(true), delay(delay), expire_time(expire_time), strength(1)
        {}
        explicit Homing(const float delay, const float expire_time, float strength) :
            targetPlayer(true), delay(delay), expire_time(expire_time), strength(strength)
        {}
        explicit Homing(const Physics::Position targetPosition, const float expire_time) :
            targetPlayer(false), targetPosition(targetPosition), delay(0), expire_time(expire_time), strength(1)
        {}
        explicit Homing(const Physics::Position targetPosition, const float delay, const float expire_time) :
            targetPlayer(false), targetPosition(targetPosition), delay(delay), expire_time(expire_time), strength(1)
        {}
        explicit Homing(
                const Physics::Position targetPosition, const float delay, const float expire_time, float strength) :
            targetPlayer(false),
            targetPosition(targetPosition),
            delay(delay),
            expire_time(expire_time),
            strength(strength)
        {}
    };
} // namespace Game::BulletHell
