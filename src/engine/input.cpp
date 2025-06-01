#include "system.h"
#include <string>
#include <format>

using System::Input;

Input& Input::instance()
{
    static Input instance;
    return instance;
}

void Input::set_key_down(uint8_t vk_code)
{
    with_input_lock
    ([vk_code](Input& instance) 
        {
            instance._keyboard_state[vk_code] = 1;
        }
    );
}

void Input::set_key_up(uint8_t vk_code)
{
    with_input_lock
    ([vk_code](Input& instance) 
        {
            instance._keyboard_state[vk_code] = 0;
        }
    );
}

bool Input::is_key_down(uint8_t vk_code)
{
    return with_input_lock
    ([vk_code](Input& instance) 
        {
            return instance._keyboard_state[vk_code];
        }
    );
}

void Input::set_mouse_position(uint16_t x, uint16_t y)
{
    with_input_lock
    ([x, y](Input& instance) 
        {
            instance.mouse_position.x = x;
            instance.mouse_position.y = y;
        }
    );
}

Math::Vector2<uint16_t> Input::get_mouse_position()
{
    return with_input_lock
    ([](Input& instance) 
        {
        return instance.mouse_position;
        }
    );
}