#pragma once

namespace Game::BulletHell
{
    struct Input
    {
        float axis_x;
        float axis_y;
        bool shift; // presumably "isFocus". will ask later
        bool z; // presumably "isInteract". will ask later
        bool x; // presumably "isCancel". will ask later
    };
}
