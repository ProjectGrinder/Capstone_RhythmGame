#pragma once

namespace Game::BulletHell
{
    struct Homing
    {

        int expire_time;
        float strength;
        int start_time;
        explicit Homing(const int expire_time = 5000, const float strength = 1.0f, const int start_time = 0) : expire_time(expire_time), strength(strength), start_time(start_time)
        {}

    };
} // namespace Game::BulletHell
