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
        Particle() : lifetime(1000), destroy_effect(Disappear)
        {}
        explicit Particle(const int lifetime, const ParticleDestroyEffect destroy_effect = Disappear) : lifetime(lifetime), destroy_effect(destroy_effect)
        {}
    };
}