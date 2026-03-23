#pragma once

namespace Game::Overview
{
    struct Input
    {
        float axis_x;
        float axis_y;
        bool shift; // run
        bool z; //jump
        bool x;
        bool tab;
        Input() : axis_x(0), axis_y(0), shift(false), z(false), x(false), tab(false)
        {}
    };
} // namespace Game::BulletHell
