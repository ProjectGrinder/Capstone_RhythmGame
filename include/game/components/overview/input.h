#pragma once

namespace Game::Overview
{
    enum class InputType
    {
        Movements = 0,
        Interact  = 1,
        Menu      = 2
    };
    struct Input
    {
        float axis_x;
        bool up_pressed;
        bool up_held;
        bool down_pressed;
        bool down_held;
        bool shift;
        bool z_pressed;
        bool z_held;
        bool x_pressed;
        bool x_held;
        bool tab_pressed;
        bool tab_held;
        bool escape_pressed;
        bool escape_held;
        Input() :
            axis_x(0),
            up_pressed(false),
            up_held(false),
            down_pressed(false),
            down_held(false),
            shift(false),
            z_pressed(false),
            z_held(false),
            x_pressed(false),
            x_held(false),
            tab_pressed(false),
            tab_held(false),
            escape_pressed(false),
            escape_held(false)
        {}
    };
} // namespace Game::Overview
