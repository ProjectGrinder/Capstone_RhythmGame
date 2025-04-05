#include <string>
#include <format>

#include "input_handler.h"

#define KEYDOWN 0x80

using System::InputHandler;

InputHandler InputHandler::_instance;

InputHandler& InputHandler::instance()
{
    return (_instance);
}

int System::InputHandler::poll()
{
    int error = ERROR_SUCCESS;
    _instance._prev_keyboard_state = _instance._current_keyboard_state;
    bool kb_ok = GetKeyboardState(_instance._current_keyboard_state.data());
    if (!kb_ok)
    {
        error = GetLastError();

        std::string buffer =
            std::format(
                "[ src/input_handler.cpp | System::InputHandler::poll ] - GetKeyboardState failed with error code {}",
                error
            );

        OutputDebugStringA(buffer.c_str());
    }

    bool mouse_ok = GetCursorPos(&_instance._mouse_position);
    if (!mouse_ok)
    {
        error = GetLastError();
        std::string buffer =
            std::format(
                "[ src/input_handler.cpp | System::InputHandler::poll ] - GetCursorPos failed with error code {}",
                error
            );

        OutputDebugStringA(buffer.c_str());
    }

    return (error);
}

bool System::InputHandler::key_pressed(BYTE virtual_key)
{
    bool is_down_now = _instance._current_keyboard_state.at(virtual_key) & KEYDOWN;
    bool was_down_before = _instance._prev_keyboard_state.at(virtual_key) & KEYDOWN;

    return (is_down_now && !was_down_before);
}

bool System::InputHandler::key_held(BYTE virtual_key)
{
    bool is_down_now = _instance._current_keyboard_state.at(virtual_key) & KEYDOWN;
    bool was_down_before = _instance._prev_keyboard_state.at(virtual_key) & KEYDOWN;

    return (is_down_now && was_down_before);
}

bool System::InputHandler::key_released(BYTE virtual_key)
{
    bool is_down_now = _instance._current_keyboard_state.at(virtual_key) & KEYDOWN;
    bool was_down_before = _instance._prev_keyboard_state.at(virtual_key) & KEYDOWN;

    return (!is_down_now && was_down_before);
}

POINT System::InputHandler::mouse_position(HWND hwnd)
{
    if (!hwnd)
    {
        return (_instance._mouse_position);
    }

    POINT client_position = _instance._mouse_position;
    bool convert_ok = ScreenToClient(hwnd, &client_position);
    if (convert_ok)
    {
        return (client_position);
    }

    int error = GetLastError();
    std::string buffer =
        std::format(
            "[ src/input_handler.cpp | System::InputHandler::mouse_position ] - ScreenToClient failed with error code {}",
            error
        );

    OutputDebugStringA(buffer.c_str());
    return (_instance._mouse_position);
}
