#pragma once

namespace Game::Rhythm
{
    struct KeyInput
    {
        bool key1_pressed;
        bool key1_hold;
        bool key2_pressed;
        bool key2_hold;
        bool key3_pressed;
        bool key3_hold;
        bool key4_pressed;
        bool key4_hold;
        KeyInput() : key1_pressed(false), key1_hold(false), key2_pressed(false), key2_hold(false),
        key3_pressed(false), key3_hold(false), key4_pressed(false), key4_hold(false)
        {}
    };
}
