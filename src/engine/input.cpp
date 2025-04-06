#include "input.h"
#include <string>
#include <format>

using System::Input;

Input Input::_instance;

Input& Input::instance()
{
    return (_instance);
}

void System::Input::set_key_down(uint8_t vkCode)
{
	_instance._keyboard_state[vkCode] = 1;
}

void System::Input::set_key_up(uint8_t vkCode)
{
	_instance._keyboard_state[vkCode] = 0;
}

bool System::Input::is_key_down(uint8_t vkCode) {
    return _instance._keyboard_state[vkCode];
}