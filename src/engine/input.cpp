#include "input.h"
#include <string>
#include <format>

using System::Input;

Input Input::_instance;

Input& Input::instance()
{
    return (_instance);
}

void System::Input::set_key_down(uint8_t vk_code)
{
	_instance._keyboard_state[vk_code] = 1;
}

void System::Input::set_key_up(uint8_t vk_code)
{
	_instance._keyboard_state[vk_code] = 0;
}

bool System::Input::is_key_down(uint8_t vk_code) 
{
    return _instance._keyboard_state[vk_code];
}