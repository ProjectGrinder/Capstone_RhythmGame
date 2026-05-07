#pragma once

namespace Game::BulletHell
{
    struct Input
    {
        float axis_x;
        float axis_y;
        bool shift; // presumably "isFocus". will ask later
        bool z;     // presumably "isInteract". will ask later
        bool z_pressed;
        bool z_hold;
        bool x;     // presumably "isCancel". will ask later
        Input() : axis_x(0), axis_y(0), shift(false), z(false), z_pressed(false), z_hold(false), x(false)
        {}
    };
} // namespace Game::BulletHell
