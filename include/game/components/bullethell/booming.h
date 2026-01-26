#pragma once

namespace Game::BulletHell
{
    // Vary Telegraph / Fadeout Style is postponed for later
    struct Booming
    {
        float max_size;
        float lifetime;

        float init_size;
        float divider; // size = (max_size - size) / divider

        Booming() : max_size(0), lifetime(0), init_size(0), divider(0)
        {}

        Booming(const float max_size, const float lifetime) : max_size(max_size), lifetime(lifetime), init_size(0), divider(1)
        {}

        Booming(const float max_size, const float lifetime, const float init_size, const float divider) :
            max_size(max_size), lifetime(lifetime), init_size(init_size), divider(divider)
        {}
    };
}
