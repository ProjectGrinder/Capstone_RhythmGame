#pragma once

/*	
 * OS polls this to get keyboard and mouse inputs
 * (or at least that's what I assume is happening)
 */

#include <Windows.h>

namespace System
{
class InputHandler
{
private:
    bool _is_running = false;
    static InputHandler _instance;

    InputHandler() {};
public:
    static InputHandler &instance();
    void poll();
};
}

