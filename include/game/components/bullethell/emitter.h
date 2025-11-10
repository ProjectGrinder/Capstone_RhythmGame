#pragma once

#include "particle.h"

namespace Game::BulletHell
{
    enum EmitterType
    {
        NONE,
    };

    struct Emitter
    {
        EmitterType type;
        int spawn_amount;
        float spawn_rate;
        int spawn_time;
        float angle_offset;
        Particle particle;
        Emitter() : type(NONE), spawn_amount(0), spawn_rate(0), spawn_time(0), angle_offset(0), particle()
        {}
        explicit Emitter(
            const EmitterType type,
            const Particle particle,
            const int spawn_amount=0,
            const float spawn_rate=0,
            const int spawn_time=0,
            const float angle_offset=0)  :
        type(type), spawn_amount(spawn_amount), spawn_rate(spawn_rate), spawn_time(spawn_time), angle_offset(angle_offset), particle(particle)
        {}
    };
}