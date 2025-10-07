#pragma once

#include "position.h"

namespace Game::Physics
{
    struct CircularCollider
    {
        Position offset;
        float radius;
        unsigned int layer;
        unsigned int mask;
    };
}