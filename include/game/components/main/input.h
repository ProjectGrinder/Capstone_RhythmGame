#pragma once

namespace Game
{
    struct Input
    {
        float axis_x;
        float axis_y;
        bool up_pressed;
        bool down_pressed;
        bool shift_pressed;
        bool shift_held;
        bool z_pressed;
        bool z_held;
        bool x_pressed;
        bool x_held;
        bool tab_pressed;
        bool tab_held;
        bool escape_pressed;
        bool escape_held;
        bool key1_pressed;
        bool key2_pressed;
        bool key3_pressed;
        bool key4_pressed;
        bool key1_held;
        bool key2_held;
        bool key3_held;
        bool key4_held;
        Input() = default;
    };
} // namespace Game::Overview
