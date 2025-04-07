#pragma once

#include <Windows.h>
#include <cstdint>

namespace System
{
    static LRESULT CALLBACK window_process(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

    class Window : public WNDCLASSEXA
    {
    private:
        HWND _hwnd = nullptr;
    public:
        Window(HINSTANCE system_instance);
        ~Window();

        uint32_t create_window();
        HWND get_handle();
    };
}