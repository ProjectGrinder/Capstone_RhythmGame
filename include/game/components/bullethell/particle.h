#pragma once

namespace Game::BulletHell
{
    enum ParticleDestroyEffect
    {
        Fade,
        Shrink,
        Disappear,
        Custom
    };
    struct Particle
    {
        int lifetime;
        ParticleDestroyEffect destroy_effect;
        explicit Particle(const int lifetime) : lifetime(lifetime), destroy_effect(Disappear)
        {}
        explicit Particle(const int lifetime, const ParticleDestroyEffect destroy_effect) :
            lifetime(lifetime), destroy_effect(destroy_effect)
        {}
    };
} // namespace Game::BulletHell
