#pragma once

namespace Game::BulletHell
{
    // Vary Telegraph / Fadeout Style is postponed for later
    struct Laser
    {
        float start_pos_x, start_pos_y;
        float length;
        float lifetime;

        float speed;

        Laser() : start_pos_x(0), start_pos_y(0), length(0), lifetime(0), speed(0)
        {}

        Laser(const float start_pos_x, const float start_pos_y,const float length, const float lifetime, const float speed) :
            start_pos_x(start_pos_x), start_pos_y(start_pos_y),length(length), lifetime(lifetime), speed(speed)
        {}

    };
}