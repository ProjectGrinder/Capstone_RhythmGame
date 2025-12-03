#include "windows_types.h"

#include <Windows.h>

extern HWND get_window_handler();

Position get_mouse_position()
{
    POINT p = {0};
    HWND handler = get_window_handler();
    Position res = {0};

    if (handler == NULL)
        goto exit;

    GetCursorPos(&p);
    ScreenToClient(handler, &p);

    res.x = p.x;
    res.y = p.y;
exit:
    return (res);
};

int get_key_state(const int vk)
{
    return ((GetAsyncKeyState(vk) & 0x8000) != 0);
}

