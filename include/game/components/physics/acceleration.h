#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Acceleration
    {
        float ax, ay, max_speed_x, max_speed_y;
        Acceleration() : ax(0), ay(0), max_speed_x(999), max_speed_y(999)
        {}
        explicit Acceleration(const float a, const float max_speed = 999) : ax(a), ay(0), max_speed_x(max_speed), max_speed_y(999) {}
        explicit Acceleration(const float ax, const float ay, const float max_speed_x, const float max_speed_y) : ax(ax), ay(ay), max_speed_x(max_speed_x), max_speed_y(max_speed_y)
        {}
    };
}
