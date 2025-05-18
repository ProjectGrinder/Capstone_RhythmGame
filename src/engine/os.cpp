#include "system.h"
#include "utils.h"

#ifndef PROJECT_NAME
#define PROJECT_NAME NO_NAME_PROJECT
#endif
#define STR(name) #name
#define TO_STR(name) STR(name)

using System::OS;

OS OS::_instance;

OS::OS()
{
    _instance._system_instance_handler = GetModuleHandleA(NULL);
    if (_instance._system_instance_handler == nullptr)
    {
        LOG_DEBUG("Error code: {}, GetModuleHandle failed.", GetLastError());
    }
}

OS& OS::instance()
{
    return(_instance);
}

uint32_t OS::run()
{
    uint32_t error = ERROR_SUCCESS;
    if (_instance._system_instance_handler == nullptr)
    {
        error = ERROR_INVALID_STATE;
        goto exit;
    }

    error = _instance._create_window();
#ifndef _TESTING
    if (error == ERROR_SUCCESS)
    {
        _instance._is_running = true;

        while (_instance._is_running)
        {
            /*  Polling Event  */
            _instance._poll_event();
            /*  Upadate Game  */

            /*  Render  */

            /*
             *  Fix framerate delay
             *  TODO: Make this high precision
             */
            _instance._sleep();
        }
    }
#endif
exit:
    return(error);
}

bool OS::is_running()
{
    return(_instance._is_running);
}

void OS::stop()
{
    _instance._is_running = false;
}

uint32_t OS::set_system_precision(int32_t ms)
{
    if (ms <= 0)
    {
        Utils::print_debug
        (
            "engine/os.cpp",
            __FUNCTION__,
            "Error code: {}, Cannot set system time precision to value less or equal to 0.",
            ERROR_INVALID_PARAMETER
        );
        return(ERROR_INVALID_PARAMETER);
    }
    else
    {
        _instance._system_precision = (uint16_t)ms;
        return(ERROR_SUCCESS);
    }
}

uint32_t OS::_sleep()
{
    LARGE_INTEGER start, end, frequency;

    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&start);

    do
    {
        QueryPerformanceCounter(&end);
        Sleep(0);
    }
    while
    (
        (end.QuadPart - start.QuadPart)
        * 1000 / frequency.QuadPart
        < _instance._system_precision
    );
    return(ERROR_SUCCESS);
}

uint32_t OS::_poll_event()
{
    MSG msg;
    while
    (
        PeekMessage
        (
            &msg,
            _instance._window_handler,
            0,0, PM_REMOVE
        )
    )
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            /* Please remove this code after main is done*/
            if (msg.wParam == VK_ESCAPE)
            {
                stop();
            }
            Input::set_key_down((uint8_t)msg.wParam);
            break;
        case WM_KEYUP:
            Input::set_key_up((uint8_t)msg.wParam);
            break;
        case WM_MOUSEMOVE:
            Input::set_mouse_position((uint16_t)msg.pt.x, (uint16_t)msg.pt.y);
            break;
        }

        DispatchMessageA(&msg);
    }
    return(ERROR_SUCCESS);
}

uint16_t OS::get_system_precision()
{
    return(_instance._system_precision);
}

uint32_t OS::_create_window()
{
    WNDCLASSEXA window_settings;
    window_settings.cbSize = sizeof(WNDCLASSEXA);
    window_settings.style = 0;
    window_settings.lpfnWndProc = window_process;
    window_settings.cbClsExtra = 0;
    window_settings.cbWndExtra = 0;
    window_settings.hInstance = _instance._system_instance_handler;
    window_settings.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    window_settings.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    window_settings.hCursor = LoadCursorA(NULL, IDC_ARROW);
    window_settings.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    window_settings.lpszMenuName = nullptr;
    window_settings.lpszClassName = TO_STR(PROJECT_NAME);

    uint32_t error = ERROR_SUCCESS;

    if (RegisterClassExA(&window_settings) == 0)
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
    
    this->_window_handler = CreateWindowExA
    (
        0, window_settings.lpszClassName, window_settings.lpszClassName,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480, nullptr, nullptr, _instance._system_instance_handler, nullptr
    );

    if (this->_window_handler == nullptr)
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

static LRESULT CALLBACK System::window_process
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
