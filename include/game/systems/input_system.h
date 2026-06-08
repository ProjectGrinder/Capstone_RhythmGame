#pragma once

#include "game/components.h"
#include "system/input.h"

namespace Game
{
    template<typename T>
    void input_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input)
    {
        if (input.begin() == input.end())
            return;

        // TODO: Change fixed key bindings to be rebindable
        constexpr auto LEFT = 0x25;
        constexpr auto UP = 0x26;
        constexpr auto RIGHT = 0x27;
        constexpr auto DOWN = 0x28;
        constexpr auto LSHIFT = 0xA0;
        constexpr auto Z = 0x5A;
        constexpr auto X = 0x58;

        constexpr auto KEY1 = 0x53; // S
        constexpr auto KEY2 = 0x44; // D
        constexpr auto KEY3 = 0x4C; // L
        constexpr auto KEY4 = 0xBA; // ;
        constexpr auto TAB = 0x09;
        constexpr auto ESC = 0x1B;

        auto &input_c = input.front().get<Input>();

        input_c.left_pressed = get_key_state(LEFT) && !input_c.left_held;
        input_c.left_held = get_key_state(LEFT);
        input_c.right_pressed = get_key_state(RIGHT) && !input_c.right_held;
        input_c.right_held = get_key_state(RIGHT);
        input_c.up_pressed = get_key_state(UP) && !input_c.up_held;
        input_c.up_held = get_key_state(UP);
        input_c.down_pressed = get_key_state(DOWN) && !input_c.down_held;
        input_c.down_held = get_key_state(DOWN);

        input_c.axis_x = static_cast<float>(get_key_state(RIGHT) - get_key_state(LEFT));

        input_c.axis_y = static_cast<float>(get_key_state(UP) - get_key_state(DOWN));

        input_c.shift_pressed = get_key_state(LSHIFT) && !input_c.shift_held;
        input_c.shift_held = get_key_state(LSHIFT);

        input_c.z_pressed = get_key_state(Z) && !input_c.z_held;
        input_c.z_held = get_key_state(Z);

        input_c.key1_pressed = get_key_state(KEY1) && !input_c.key1_held;
        input_c.key1_held = get_key_state(KEY1);

        input_c.key2_pressed = get_key_state(KEY2) && !input_c.key2_held;
        input_c.key2_held = get_key_state(KEY2);

        input_c.key3_pressed = get_key_state(KEY3) && !input_c.key3_held;
        input_c.key3_held = get_key_state(KEY3);

        input_c.key4_pressed = get_key_state(KEY4) && !input_c.key4_held;
        input_c.key4_held = get_key_state(KEY4);

        input_c.x_pressed = get_key_state(X) && !input_c.x_held;
        input_c.x_held = get_key_state(X);

        input_c.tab_pressed = get_key_state(TAB) && !input_c.tab_held;
        input_c.tab_held = get_key_state(TAB);

        input_c.escape_pressed = get_key_state(ESC) && !input_c.escape_held;
        input_c.escape_held = get_key_state(ESC);
    }
} // namespace Game::Battle
