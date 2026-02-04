#pragma once

namespace Game::BulletHell
{
    // Vary Telegraph / Fadeout Style is postponed for later
    struct Laser
    {
        float start_pos_x, start_pos_y;
        float length;

        float speed;
        bool is_activated = false;

        Laser() : start_pos_x(0), start_pos_y(0), length(0), speed(1)
        {}

        Laser(const float start_pos_x, const float start_pos_y,const float length) :
            start_pos_x(start_pos_x), start_pos_y(start_pos_y), length(length), speed(1)
        {}

        Laser(const float start_pos_x, const float start_pos_y,const float length, const float speed) :
            start_pos_x(start_pos_x), start_pos_y(start_pos_y), length(length), speed(speed)
        {}

    };
}