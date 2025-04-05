#pragma once

/*	
 * OS polls this to get keyboard and mouse inputs
 * (or at least that's what I assume is happening)
 */

#include <Windows.h>
#include <array>

namespace System
{
class InputHandler
{
private:
    bool _is_running = false;
    static InputHandler _instance;
    std::array<BYTE, 256> _prev_keyboard_state{};
    std::array<BYTE, 256> _current_keyboard_state{};
    POINT _mouse_position{};
    InputHandler() {};
public:
    static InputHandler &instance();
    int poll();
    bool key_pressed(BYTE);
    bool key_held(BYTE);
    bool key_released(BYTE);
    POINT mouse_position(HWND);
};
}

