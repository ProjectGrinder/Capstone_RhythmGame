#include "system.h"

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
    return (_instance._keyboard_state[vk_code]);
}

void System::Input::set_mouse_position(uint16_t x, uint16_t y)
{
    _instance.mouse_position.x = x;
    _instance.mouse_position.y = y;
}

Math::Vector2<uint16_t> System::Input::get_mouse_position() 
{ 
    return (_instance.mouse_position);
}