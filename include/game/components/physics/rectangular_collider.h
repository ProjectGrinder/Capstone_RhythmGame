#pragma once

#include "base_collider.h"
#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct RectangularCollider : BaseCollider
    {
        Position size;
        explicit RectangularCollider(
                const Position &offset,
                const Position &size,
                const unsigned int layer = 0,
                const unsigned int mask = 0) :
            size(size), BaseCollider(offset, layer, mask)
        {}
    };
} // namespace Game::Physics
