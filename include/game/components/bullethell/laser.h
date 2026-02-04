#pragma once

namespace Game::BulletHell
{
    // Vary Telegraph / Fadeout Style is postponed for later
    struct Laser
    {
        float start_pos_x, start_pos_y;
        float length;

        int boom_frame;
        bool is_activated = false;

        Laser() : start_pos_x(0), start_pos_y(0), length(0), boom_frame(1)
        {}

        Laser(const float start_pos_x, const float start_pos_y,const float length) :
            start_pos_x(start_pos_x), start_pos_y(start_pos_y), length(length), boom_frame(1)
        {}

        Laser(const float start_pos_x, const float start_pos_y,const float length, const int boom_frame) :
            start_pos_x(start_pos_x), start_pos_y(start_pos_y), length(length), boom_frame(boom_frame)
        {}

    };
}