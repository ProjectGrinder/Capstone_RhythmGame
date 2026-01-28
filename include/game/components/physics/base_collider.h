#pragma once


#include "position.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct BaseCollider
    {
        float offset_x;
        float offset_y;
        unsigned int layer;
        unsigned int mask;
        BaseCollider() : offset_x(0),offset_y(0), layer(0), mask(0)
        {}
        explicit BaseCollider(
                const float &offset_x, const float &offset_y, const unsigned int layer = 0, const unsigned int mask = 0) :
            offset_x(offset_x), offset_y(offset_y), layer(layer), mask(mask)
        {}
    };
}