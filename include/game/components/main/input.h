#pragma once

namespace Game
{
    struct Input
    {
        float axis_x;
        float axis_y;
        bool left_pressed;
        bool left_held;
        bool right_pressed;
        bool right_held;
        bool up_pressed;
        bool up_held;
        bool down_pressed;
        bool down_held;
        bool e_pressed;
        bool e_held;
        bool shift_pressed;
        bool shift_held;
        bool space_pressed;
        bool space_held;
        bool tab_pressed;
        bool tab_held;
        bool escape_pressed;
        bool escape_held;
        bool enter_pressed;
        bool enter_held;
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
