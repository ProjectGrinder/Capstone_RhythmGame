#include "input_handler.h"
#include <string>
#include <format>

using System::InputHandler;

InputHandler InputHandler::_instance;

InputHandler& InputHandler::instance()
{
    return (_instance);
}

void System::InputHandler::handle(UINT message, BYTE vkCode)
{
    if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN) {
        _instance._keyboard_state[vkCode] = true;
    }
    else if (message == WM_KEYUP || message == WM_SYSKEYUP) {
        _instance._keyboard_state[vkCode] = false;
    }
}

bool System::InputHandler::isKeyDown(BYTE vkCode) const {
    return _instance._keyboard_state[vkCode];
}