#pragma once

#include <array>
#include "maths/vector2.h"
#include <mutex>

namespace System
{
    class Input
    {
    private:
        std::array<uint8_t, 256> _keyboard_state{};
        Math::Vector2<uint16_t> mouse_position;

        // This may be a REALLY dumb idea
        std::mutex _mutex;

        Input() {};

        // Helper for said REALLY dumb idea
        template<typename Func>
        static auto with_input_lock(Func&& func) {
            auto& instance = Input::instance();
            std::lock_guard<std::mutex> lock(instance._mutex);
            return func(instance);
        }
    public:
        static Input& instance();
        static void set_key_down(uint8_t vk_code);
        static void set_key_up(uint8_t vk_code);
        static bool is_key_down(uint8_t vk_code);
        static void set_mouse_position(uint16_t x, uint16_t y);
        static Math::Vector2<uint16_t> get_mouse_position();
    };
}
