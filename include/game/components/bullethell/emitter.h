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
    };
}