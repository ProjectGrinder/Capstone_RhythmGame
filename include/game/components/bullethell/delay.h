#pragma once

namespace Game::BulletHell
{
    struct Delay
    {
        int delay;
        Delay() : delay(0)
        {}
        explicit Delay(const float delay) : delay(delay)
        {}
    };
}