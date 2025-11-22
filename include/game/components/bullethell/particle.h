#pragma once

namespace Game::BulletHell
{
    struct Particle
    {
        int lifetime;
        Particle() : lifetime(0)
        {}
        explicit Particle(const int lifetime) : lifetime(lifetime)
        {}
    };
}