#pragma once

#include "base_collider.h"
#include "position.h"

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
            radius_x(radius), radius_y(radius), BaseCollider(0,0, layer, mask)
        {
        }
        explicit CircularCollider(
                const float &offset_x, const float &offset_y, const float radius, const unsigned int layer = 0, const unsigned int mask = 0) :
            radius_x(radius), radius_y(radius), BaseCollider(offset_x, offset_y, layer, mask)
        {
        }
        explicit CircularCollider(
                const float radius_x, const float radius_y, const unsigned int layer = 0, const unsigned int mask = 0) :
            radius_x(radius_x), radius_y(radius_y), BaseCollider(0,0, layer, mask)
        {
        }
        explicit CircularCollider(
                const float &offset_x, const float &offset_y, const float radius_x, const float radius_y, const unsigned int layer = 0, const unsigned int mask = 0) :
           radius_x(radius_x), radius_y(radius_y), BaseCollider(offset_x, offset_y, layer, mask)
        {
        }

        void change_radius(const float new_radius)
        {
            radius_x = new_radius;
            radius_y = new_radius;
        }
    };
}