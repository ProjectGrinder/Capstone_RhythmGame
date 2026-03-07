#pragma once


#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct BaseCollider
    {
        Position offset;
        unsigned int layer;
        unsigned int mask;
        BaseCollider() : layer(0), mask(0)
        {}
        explicit BaseCollider(const Position &offset, const unsigned int layer = 0, const unsigned int mask = 0) :
            offset(offset), layer(layer), mask(mask)
        {}
    };
} // namespace Game::Physics
