#pragma once

#include "base_collider.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct RectangularCollider: BaseCollider
    {
        float size_x;
        float size_y;
        RectangularCollider() : BaseCollider(0, 0, 0, 0), size_x(1), size_y(1)
        {}
        explicit RectangularCollider(
                const float &size_x, const float &size_y, const unsigned int layer = 0, const unsigned int mask = 0) :
            BaseCollider(0, 0, layer, mask), size_x(size_x), size_y(size_y)
        {}
        explicit RectangularCollider(
                const float &offset_x, const float &offset_y, const float &size_x, const float &size_y, const unsigned int layer = 0, const unsigned int mask = 0) :
            BaseCollider(offset_x, offset_y, layer, mask), size_x(size_x), size_y(size_y)
        {}
    };
}