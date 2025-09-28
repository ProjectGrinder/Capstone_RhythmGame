#pragma once

#include "position.h"

namespace Game::BulletHell
{
    struct Collider
    {
        Position offset;
        float radius;
        unsigned int layer;
        unsigned int mask;
    };
}