#pragma once

#include "position.h"

namespace Game::Physics
{
    struct CircularCollider
    {
        Position offset;
        Position size;
        unsigned int layer;
        unsigned int mask;
        CircularCollider() : layer(0), mask(0)
        {}
        explicit CircularCollider(
                const Position &offset, const Position &size, const unsigned int layer = 0, const unsigned int mask = 0) :
            offset(offset), size(size), layer(layer), mask(mask)
        {}
    };
}