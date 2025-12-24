#pragma once

#include "game/components.h"
#include "system/input.h"

namespace Game::Battle
{
    template <typename T>
    void InputSystem([[maybe_unused]] T &syscall, System::ECS::Query<BattleState> &query1, System::ECS::Query<BulletHell::Input> &bullet_hell_input, System::ECS::Query<Rhythm::KeyInput> &rhythm_input)
    {
        // TODO: Change fixed key bindings to be rebindable
        constexpr auto VK_LEFT = 0x25;
        constexpr auto VK_UP = 0x26;
        constexpr auto VK_RIGHT = 0x27;
        constexpr auto VK_DOWN = 0x28;
        constexpr auto VK_LSHIFT = 0xA0;
        constexpr auto Z = 0x5A;
        constexpr auto X = 0x58;

        constexpr auto D = 0x44;
        constexpr auto F = 0x46;
        constexpr auto J = 0x4A;
        constexpr auto K = 0x4B;

        switch (query1.front().get<BattleState>().current_phase)
        {
        case BULLET_HELL:
            bullet_hell_input.front().get<BulletHell::Input>().axis_x = static_cast<float>(get_key_state(VK_LEFT) - get_key_state(VK_RIGHT));
            bullet_hell_input.front().get<BulletHell::Input>().axis_y = static_cast<float>(get_key_state(VK_UP) - get_key_state(VK_DOWN));
            bullet_hell_input.front().get<BulletHell::Input>().x = get_key_state(X);
            bullet_hell_input.front().get<BulletHell::Input>().z = get_key_state(Z);
            bullet_hell_input.front().get<BulletHell::Input>().shift = get_key_state(VK_LSHIFT);
            break;
        case RHYTHM:
            rhythm_input.front().get<Rhythm::KeyInput>().input1 = get_key_state(D);
            rhythm_input.front().get<Rhythm::KeyInput>().input2 = get_key_state(F);
            rhythm_input.front().get<Rhythm::KeyInput>().input3 = get_key_state(J);
            rhythm_input.front().get<Rhythm::KeyInput>().input4 = get_key_state(K);
            break;
        default:
            break;
        }
    }
}