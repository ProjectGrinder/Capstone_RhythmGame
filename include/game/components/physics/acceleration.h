#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Acceleration
    {
        float ax, ay, max_speed;
        Acceleration() : ax(0), ay(0), max_speed(0)
        {}
        explicit Acceleration(const float ax, const float ay) : ax(ax), ay(ay), max_speed(0)
        {}
        explicit Acceleration(const float ax, const float ay, float max_speed) : ax(ax), ay(ay), max_speed(max_speed)
        {}
    };
}
