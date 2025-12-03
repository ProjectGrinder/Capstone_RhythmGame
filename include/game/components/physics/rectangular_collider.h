#pragma once

#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct RectangularCollider
    {
        Position offset;
        Position size;
        unsigned int layer;
        unsigned int mask;
        RectangularCollider() : layer(0), mask(0)
        {}
        explicit RectangularCollider(
                const Position &offset, const Position &size, const unsigned int layer = 0, const unsigned int mask = 0) :
            offset(offset), size(size), layer(layer), mask(mask)
        {}
    };
}