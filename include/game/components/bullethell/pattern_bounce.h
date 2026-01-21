#pragma once

namespace Game::BulletHell
{
    struct Bounce
    {
        int bounce_time;
        //                       Left, Right, Up , Down
        bool reflect_side[4] = { true, true, true, true };
        Bounce() : bounce_time(1)
        {}
        explicit Bounce(const int bounce_time, const bool reflect_side[4]) : bounce_time(bounce_time)
        {
            for (int i = 0; i < 4; ++i)
                this->reflect_side[i] = reflect_side[i];
        }
    };
}