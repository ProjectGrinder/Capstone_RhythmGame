#pragma once

#include "base_collider.h"
#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct CircularCollider : BaseCollider
    {
        float radius;
        CircularCollider() : radius(0)
        {}
        explicit CircularCollider(
                const Position &offset, const float radius, const unsigned int layer = 0, const unsigned int mask = 0) :
            radius(radius), BaseCollider(offset, layer, mask)
        {}
    };
} // namespace Game::Physics
