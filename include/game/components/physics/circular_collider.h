#pragma once

#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct CircularCollider
    {
        Position offset;
        float radius;
        unsigned int layer;
        unsigned int mask;
        CircularCollider() : radius(0), layer(0), mask(0)
        {}
        explicit CircularCollider(
                const Position &offset, const float radius, const unsigned int layer = 0, const unsigned int mask = 0) :
            offset(offset), radius(radius), layer(layer), mask(mask)
        {}
    };
}