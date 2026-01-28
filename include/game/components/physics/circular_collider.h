#pragma once

#include "base_collider.h"

// FIXME: May separate thread
namespace Game::Physics
{
    struct CircularCollider : BaseCollider
    {
        float radius_x, radius_y;
        CircularCollider() : radius_x(0), radius_y(0)
        {}
        explicit CircularCollider(
                const float radius, const unsigned int layer = 0, const unsigned int mask = 0) :
            BaseCollider(0, 0, layer, mask), radius_x(radius), radius_y(radius)
        {
        }
        explicit CircularCollider(
                const float &offset_x, const float &offset_y, const float radius, const unsigned int layer = 0, const unsigned int mask = 0) :
            BaseCollider(offset_x, offset_y, layer, mask), radius_x(radius), radius_y(radius)
        {
        }
        explicit CircularCollider(
                const float radius_x, const float radius_y, const unsigned int layer = 0, const unsigned int mask = 0) :
            BaseCollider(0, 0, layer, mask), radius_x(radius_x), radius_y(radius_y)
        {
        }
        explicit CircularCollider(
                const float &offset_x, const float &offset_y, const float radius_x, const float radius_y, const unsigned int layer = 0, const unsigned int mask = 0) :
           BaseCollider(offset_x, offset_y, layer, mask), radius_x(radius_x), radius_y(radius_y)
        {
        }

        void change_radius(const float new_radius)
        {
            radius_x = new_radius;
            radius_y = new_radius;
        }
    };
}