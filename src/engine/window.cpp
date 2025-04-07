#include "system.h"
#include "utils.h"

#ifndef PROJECT_NAME
#define PROJECT_NAME NO_NAME_PROJECT
#endif
#define STR(name) #name
#define TO_STR(name) STR(name)

namespace System {
    static LRESULT CALLBACK window_process
    (
        HWND hwnd,
        UINT msg,
        WPARAM w_param,
        LPARAM l_param
    )
    {
        switch(msg)
        {
        case WM_CLOSE:
            OS::stop();
            PostQuitMessage(0);
            return(0);
        default:
            return(DefWindowProc(hwnd, msg, w_param, l_param));
        }
    }

    Window::Window(HINSTANCE system_instance)
    {
        this->cbSize = sizeof(WNDCLASSEXA);
        this->style = 0;
        this->lpfnWndProc = window_process;
        this->cbClsExtra = 0;
        this->cbWndExtra = 0;
        this->hInstance = system_instance;
        this->hIcon = LoadIconA(NULL, IDI_APPLICATION);
        this->hIconSm = LoadIconA(NULL, IDI_APPLICATION);
        this->hCursor = LoadCursorA(NULL, IDC_ARROW);
        this->hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
        this->lpszMenuName = nullptr;
        this->lpszClassName = TO_STR(PROJECT_NAME);
    }

    Window::~Window()
    {
        if (_hwnd && IsWindow(_hwnd))
        {
            DestroyWindow(_hwnd);
        }
    }
    
    uint32_t Window::create_window()
    {
        uint32_t error = ERROR_SUCCESS;

        if (RegisterClassExA(this) == 0)
        {
            error = GetLastError();
            Utils::print_debug
            (
                "engine/window.cpp",
                "Window::create_window",
                "Error code: {}, Failed to register window.",
                error
            );
            return(error);
        }
        
        _hwnd = CreateWindowExA
        (
            0, this->lpszClassName, this->lpszClassName,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
            640, 480, nullptr, nullptr, this->hInstance, nullptr
        );

        if (_hwnd == nullptr)
        {
            error = GetLastError();
            Utils::print_debug
            (
                "engine/window.cpp",
                "Window::create_window",
                "Error code: {}, Failed to create window.",
                error
            );
            
            return(error);
        }
        return(error);
    }
    HWND Window::get_handle()
    {
        return(this->_hwnd);
    }
}

