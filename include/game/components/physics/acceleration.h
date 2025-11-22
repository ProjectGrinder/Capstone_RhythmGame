#pragma once

namespace Game::Physics
{
    struct Acceleration
    {
        float ax, ay;
        Acceleration() : ax(0), ay(0)
        {}
        explicit Acceleration(const float ax, const float ay) : ax(ax), ay(ay)
        {}
    };
}
