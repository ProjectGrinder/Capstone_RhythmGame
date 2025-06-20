#pragma once

#include <array>
#include "maths/vector2.h"

namespace System
{
    class Input
    {
    private:
        static Input _instance;
        std::array<uint8_t, 256> _keyboard_state{};
        Math::Vector2<uint16_t> mouse_position;
        Input() = default;
    public:
        static Input& instance();
        static void set_key_down(uint8_t vk_code);
        static void set_key_up(uint8_t vk_code);
        static bool is_key_down(uint8_t vk_code);
        static void set_mouse_position(uint16_t x, uint16_t y);
        static Math::Vector2<uint16_t> get_mouse_position();
    };
}
