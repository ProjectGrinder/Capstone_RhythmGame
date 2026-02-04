#pragma once

namespace Game::BulletHell
{
    // Vary Telegraph / Fadeout Style is postponed for later
    struct Booming
    {
        float max_size;

        int boom_frame;
        bool is_activated = false;

        Booming(): max_size(0), boom_frame(1)
        {}

        explicit Booming(const float max_size,  const int boom_frame = 1) :
            max_size(max_size), boom_frame(boom_frame)
        {}
    };
}
