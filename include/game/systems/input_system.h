#pragma once

#include "game/components.h"
#include "system/input.h"
#include "utils/print_debug.h"

namespace Game::Battle
{
    template<typename T>
    void input_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<BattleState> &query1,
            System::ECS::Query<BulletHell::Input> &bullet_hell_input,
            System::ECS::Query<Rhythm::KeyInput> &rhythm_input)
    {
        if (query1.begin() == query1.end())
            return;

        if (bullet_hell_input.begin() == bullet_hell_input.end() || rhythm_input.begin() == rhythm_input.end())
            return;

        // TODO: Change fixed key bindings to be rebindable
        constexpr auto LEFT = 0x25;
        constexpr auto UP = 0x26;
        constexpr auto RIGHT = 0x27;
        constexpr auto DOWN = 0x28;
        constexpr auto LSHIFT = 0xA0;
        constexpr auto Z = 0x5A;
        constexpr auto X = 0x58;

        constexpr auto D = 0x44;
        constexpr auto F = 0x46;
        constexpr auto J = 0x4A;
        constexpr auto K = 0x4B;

        switch (query1.front().get<BattleState>().current_phase)
        {
        case BULLET_HELL:
            if (bullet_hell_input.begin() == bullet_hell_input.end())
            {
                break;
            }
            bullet_hell_input.front().get<BulletHell::Input>().axis_x =
                    static_cast<float>(get_key_state(RIGHT) - get_key_state(LEFT));
            bullet_hell_input.front().get<BulletHell::Input>().axis_y =
                    static_cast<float>(get_key_state(UP) - get_key_state(DOWN));
            bullet_hell_input.front().get<BulletHell::Input>().x = get_key_state(X);
            bullet_hell_input.front().get<BulletHell::Input>().z = get_key_state(Z);
            bullet_hell_input.front().get<BulletHell::Input>().shift = get_key_state(LSHIFT);
            break;
        case RHYTHM:
            if (rhythm_input.begin() == rhythm_input.end())
            {
                break;
            }

            if (get_key_state(D))
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key1_hold = true;
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key1_pressed &&
                    !rhythm_input.front().get<Rhythm::KeyInput>().key1_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key1_pressed = true;
                }
            }
            else
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key1_hold = false;
                rhythm_input.front().get<Rhythm::KeyInput>().key1_lock = false;
            }
            if (rhythm_input.front().get<Rhythm::KeyInput>().key1_pressed)
            {
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key1_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key1_lock = true;
                }
                else
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key1_pressed = false;
                }
            }

            if (get_key_state(F))
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key2_hold = true;
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key2_pressed &&
                    !rhythm_input.front().get<Rhythm::KeyInput>().key2_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key2_pressed = true;
                }
            }
            else
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key2_hold = false;
                rhythm_input.front().get<Rhythm::KeyInput>().key2_lock = false;
            }
            if (rhythm_input.front().get<Rhythm::KeyInput>().key2_pressed)
            {
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key2_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key2_lock = true;
                }
                else
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key2_pressed = false;
                }
            }

            if (get_key_state(J))
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key3_hold = true;
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key3_pressed &&
                    !rhythm_input.front().get<Rhythm::KeyInput>().key3_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key3_pressed = true;
                }
            }
            else
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key3_hold = false;
                rhythm_input.front().get<Rhythm::KeyInput>().key3_lock = false;
            }
            if (rhythm_input.front().get<Rhythm::KeyInput>().key3_pressed)
            {
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key3_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key3_lock = true;
                }
                else
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key3_pressed = false;
                }
            }

            if (get_key_state(K))
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key4_hold = true;
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key4_pressed &&
                    !rhythm_input.front().get<Rhythm::KeyInput>().key4_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key4_pressed = true;
                }
            }
            else
            {
                rhythm_input.front().get<Rhythm::KeyInput>().key4_hold = false;
                rhythm_input.front().get<Rhythm::KeyInput>().key4_lock = false;
            }
            if (rhythm_input.front().get<Rhythm::KeyInput>().key4_pressed)
            {
                if (!rhythm_input.front().get<Rhythm::KeyInput>().key4_lock)
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key4_lock = true;
                }
                else
                {
                    rhythm_input.front().get<Rhythm::KeyInput>().key4_pressed = false;
                }
            }
            break;
        default:
            break;
        }

        // Debug key inputs
        /*
        if (bullet_hell_input.front().get<BulletHell::Input>().axis_x > 0)
        {
            LOG_INFO("BulletHell LEFT MOVEMENT");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().axis_x < 0)
        {
            LOG_INFO("BulletHell RIGHT MOVEMENT");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().axis_y > 0)
        {
            LOG_INFO("BulletHell UP MOVEMENT");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().axis_y < 0)
        {
            LOG_INFO("BulletHell DOWN MOVEMENT");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().shift)
        {
            LOG_INFO("BulletHell FOCUS");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().z)
        {
            LOG_INFO("BulletHell Z");
        }
        if (bullet_hell_input.front().get<BulletHell::Input>().x)
        {
            LOG_INFO("BulletHell X");
        }
        if (rhythm_input.front().get<Rhythm::KeyInput>().key1_pressed)
        {
            LOG_INFO("Rhythm KEY1");
        }
        if (rhythm_input.front().get<Rhythm::KeyInput>().key2_pressed)
        {
            LOG_INFO("Rhythm KEY2");
        }
        if (rhythm_input.front().get<Rhythm::KeyInput>().key3_pressed)
        {
            LOG_INFO("Rhythm KEY3");
        }
        if (rhythm_input.front().get<Rhythm::KeyInput>().key4_pressed)
        {
            LOG_INFO("Rhythm KEY4");
        }
        */
    }
} // namespace Game::Battle
