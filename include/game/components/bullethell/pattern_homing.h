#pragma once

namespace Game::BulletHell
{
    struct Homing
    {
        float expire_time;
        float strength;
        Homing() : expire_time(0.0f), strength(0.0f) {}
        explicit Homing(const float expire_time, const float strength = 1.0f) :expire_time(expire_time), strength(1)
        {}

    };
}