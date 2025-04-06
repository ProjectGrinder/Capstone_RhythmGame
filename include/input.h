#pragma once

/*	
 * OS polls this to get keyboard and mouse inputs
 * (or at least that's what I assume is happening)
 */
#include <array>

namespace System
{
    class Input
    {
    private:
        static Input _instance;
        std::array<uint8_t, 256> _keyboard_state{};
        Input() {};
    public:
        static Input& instance();
		static void set_key_down(uint8_t vk_code);
		static void set_key_up(uint8_t vk_code);
        static bool is_key_down(uint8_t vk_code);
    };
}
