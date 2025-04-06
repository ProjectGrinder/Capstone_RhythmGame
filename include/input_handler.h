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
        static InputHandler _instance;
        std::array<BYTE, 256> _keyboard_state{};
        InputHandler() {};
    public:
        static InputHandler& instance();
        void handle(UINT message, BYTE vkCode);
        bool isKeyDown(BYTE vkCode) const;
    };
}
