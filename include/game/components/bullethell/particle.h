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
        int destroy_frame;
        Particle() : lifetime(1000), destroy_effect(Disappear), destroy_frame(500)
        {}
        explicit Particle(const int lifetime, const ParticleDestroyEffect destroy_effect = Disappear, const int _destroy_frame) : lifetime(lifetime), destroy_effect(destroy_effect), destroy_frame(_destroy_frame)
        {}
    };
} // namespace Game::BulletHell
