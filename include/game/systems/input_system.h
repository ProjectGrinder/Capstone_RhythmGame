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

        constexpr auto W = 0x57;
        constexpr auto A = 0x41;
        constexpr auto S = 0x53;
        constexpr auto D = 0x44;
        constexpr auto Z = 0x5A;
        // constexpr auto X = 0x58;

        constexpr auto KEY1 = 0x58; // X
        constexpr auto KEY2 = 0x43; // C
        constexpr auto KEY3 = 0x4D; // M
        constexpr auto KEY4 = 0xBC; // ,

        constexpr auto ESC = 0x1B;
        constexpr auto SPACE = 0x20;
        constexpr auto TAB = 0x09;
        constexpr auto ENTER = 0x0D;
        constexpr auto SHIFT = 0x10;

        auto &input_c = input.front().get<Input>();

        input_c.left_pressed = (get_key_state(LEFT) || get_key_state(A)) && !input_c.left_held;
        input_c.left_held = get_key_state(LEFT) || get_key_state(A);
        input_c.right_pressed = (get_key_state(RIGHT) || get_key_state(D)) && !input_c.right_held;
        input_c.right_held = get_key_state(RIGHT) || get_key_state(D);
        input_c.up_pressed = (get_key_state(UP) || get_key_state(W)) && !input_c.up_held;
        input_c.up_held = get_key_state(UP) || get_key_state(W);
        input_c.down_pressed = (get_key_state(DOWN) || get_key_state(S)) && !input_c.down_held;
        input_c.down_held = get_key_state(DOWN) || get_key_state(S);

        input_c.axis_x = static_cast<float>((get_key_state(RIGHT) || get_key_state(D)) - (get_key_state(LEFT) || get_key_state(A)));

        input_c.axis_y = static_cast<float>((get_key_state(UP) || get_key_state(W)) - (get_key_state(DOWN) || get_key_state(S)));

        input_c.z_pressed = get_key_state(Z) && !input_c.z_held;
        input_c.z_held = get_key_state(Z);

        input_c.shift_pressed = get_key_state(SHIFT) && !input_c.shift_held;
        input_c.shift_held = get_key_state(SHIFT);

        input_c.space_pressed = get_key_state(SPACE) && !input_c.space_held;
        input_c.space_held = get_key_state(SPACE);

        input_c.key1_pressed = get_key_state(KEY1) && !input_c.key1_held;
        input_c.key1_held = get_key_state(KEY1);

        input_c.key2_pressed = get_key_state(KEY2) && !input_c.key2_held;
        input_c.key2_held = get_key_state(KEY2);

        input_c.key3_pressed = get_key_state(KEY3) && !input_c.key3_held;
        input_c.key3_held = get_key_state(KEY3);

        input_c.key4_pressed = get_key_state(KEY4) && !input_c.key4_held;
        input_c.key4_held = get_key_state(KEY4);

        input_c.tab_pressed = get_key_state(TAB) && !input_c.tab_held;
        input_c.tab_held = get_key_state(TAB);

        input_c.escape_pressed = get_key_state(ESC) && !input_c.escape_held;
        input_c.escape_held = get_key_state(ESC);

        input_c.enter_pressed = get_key_state(ENTER) && !input_c.enter_held;
        input_c.enter_held = get_key_state(ENTER);
    }
} // namespace Game::Battle
