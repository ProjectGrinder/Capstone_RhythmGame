#include "system.h"
#include "utils.h"

#ifndef PROJECT_NAME
#define PROJECT_NAME NO_NAME_PROJECT
#endif
#define STR(name) #name
#define TO_STR(name) STR(name)

using System::OS;

OS::OS(): _monitor{ 0, 0 }, _window{ 1280, 720, 0, DisplayType::WINDOW, false }
{
    this->_handler = GetModuleHandleA(NULL);
    if (this->_handler == nullptr)
    {
        LOG_DEBUG("Error code: {}, GetModuleHandle failed.", GetLastError());
        throw std::runtime_error("Unable to get current instance handler.\nMore detail please check on logs");
    }
}

OS& OS::instance()
{
    static OS _instance;
    return(_instance);
}

uint32_t OS::run()
{
    uint32_t error = ERROR_SUCCESS;

    error = instance()._create_window();
#ifndef _TESTING
    if (error == ERROR_SUCCESS)
    {
        instance()._run();
    }
#endif
    return(error);
}

bool OS::is_running()
{
    return(instance()._window.is_running);
}

void OS::stop()
{
    instance()._window.is_running = false;
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
        instance()._system_precision = (uint16_t)ms;
        return(ERROR_SUCCESS);
    }
}

uint32_t OS::_sleep() const
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
        < this->_system_precision
    );
    return(ERROR_SUCCESS);
}

void OS::_run()
{
    this->_window.is_running = true;

    while (this->_window.is_running)
    {
        /*  Polling Event  */
        this->_poll_event();
        /*  Upadate Game  */

        /*  Render  */

        this->_sleep();
    }
}

uint32_t OS::_poll_event() const
{
    MSG msg;
    while
    (
        PeekMessage
        (
            &msg,
            this->_window.handler,
            0, 0, PM_REMOVE
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
    return(instance()._system_precision);
}

uint32_t OS::_create_window()
{
    MONITORINFO monitor_info = { 0 };
    WNDCLASSEXA window_settings = { 0 };
    DEVMODEA devmode_screen_settings = { 0 };

    DWORD style_flags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

    window_settings.cbSize = sizeof(WNDCLASSEXA);
    window_settings.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_settings.lpfnWndProc = window_process;
    window_settings.cbClsExtra = 0;
    window_settings.cbWndExtra = 0;
    window_settings.hInstance = this->_handler;
    window_settings.hIcon = LoadIconA(NULL, IDI_WINLOGO);
    window_settings.hIconSm = LoadIconA(NULL, IDI_WINLOGO);
    window_settings.hCursor = LoadCursorA(NULL, IDC_ARROW);
    window_settings.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    window_settings.lpszMenuName = nullptr;
    window_settings.lpszClassName = TO_STR(PROJECT_NAME);

    monitor_info.cbSize = sizeof(MONITORINFO);

    uint32_t error = ERROR_SUCCESS;

    if (RegisterClassExA(&window_settings) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, Failed to register window.", error);
        goto Exit;
    }

    this->_window.handler = CreateWindowExA
    (
        0, window_settings.lpszClassName, window_settings.lpszClassName,
        style_flags, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, nullptr, nullptr, this->_handler, nullptr
    );

    if (this->_window.handler == nullptr)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, Failed to create window.", error);
        goto Exit;
    }

    /* We can do this because MonitorFromWindow Guarantee the return value */
    if (GetMonitorInfoA(MonitorFromWindow(this->_window.handler, MONITOR_DEFAULTTOPRIMARY),&monitor_info) == 0)
    {
        error = ERROR_MONITOR_NO_DESCRIPTOR;

        LOG_DEBUG("Error code: {}, GetMonitorInfo failed.", error);
        goto Exit;
    }

    this->_monitor.height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
    this->_monitor.width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;

    if (this->_window.width == 0)
    {
        this->_window.width = this->_monitor.width;
    }

    if (this->_window.height == 0)
    {
        this->_window.height = this->_monitor.height;
    }

    if (this->_window.width > this->_monitor.width || this->_window.height > this->_monitor.height)
    {
        LOG_DEBUG("Either window width or window height is larger than monitor's value. Resetting");

        this->_window.width = this->_monitor.width;
        this->_window.height = this->_monitor.height;
    }

    LOG_DEBUG("Monitor: {}x{}", this->_monitor.width, this->_monitor.height);
    LOG_DEBUG("Window: {}x{}", this->_window.width, this->_window.height);

    switch (this->_window.display_type)
    {
    case FULLSCREEN:

        devmode_screen_settings.dmSize = sizeof(DEVMODEA);
        devmode_screen_settings.dmPelsWidth = this->_window.width;
        devmode_screen_settings.dmPelsHeight = this->_window.height;
        devmode_screen_settings.dmBitsPerPel = 32;
        devmode_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        error = ChangeDisplaySettingsA(&devmode_screen_settings, CDS_FULLSCREEN);

        if (error != DISP_CHANGE_SUCCESSFUL)
        {
            LOG_DEBUG("Error code: {}, ChangeDisplaySettings failed.", error);

            goto Exit;
        }

        if
        (
            SetWindowPos
            (
                this->_window.handler, HWND_TOP,
                monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                this->_window.width, this->_window.height,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            ) == 0
        )
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case BORDERLESS:

        if
        (
            SetWindowPos
            (
                this->_window.handler, HWND_TOP,
                monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                this->_monitor.width, this->_monitor.height,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            ) == 0
        )
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case WINDOW:

        if
        (
            SetWindowLongPtrA
            (
                this->_window.handler,
                GWL_STYLE,
                (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
            ) == 0
        )
        {
            error = GetLastError();
            
            LOG_DEBUG("Error code: {}, SetWindowLongPtr failed.", error);
            goto Exit;
        }

        if
        (
            SetWindowPos
            (
                this->_window.handler, HWND_TOP,
                (this->_monitor.width - this->_window.width) / 2,
                (this->_monitor.height - this->_window.height) / 2,
                this->_window.width, this->_window.height,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            ) == 0
        )
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    default:

        error = ERROR_INVALID_PARAMETER;
        LOG_DEBUG("Error code: {}, Unknown display type.", error);
        goto Exit;

    }

    ShowWindow(this->_window.handler, SW_SHOW);

Exit:
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

uint32_t OS::clean_up()
{
    uint32_t error = ERROR_SUCCESS;
    if (DestroyWindow(instance()._window.handler) == 0)
    {
        error = GetLastError();

        LOG_DEBUG("Error code: {}, Unable to destroy window.", error);
    }

    if (instance()._window.display_type != FULLSCREEN)
    {
        goto Exit;
    }

    error = ChangeDisplaySettingsA(nullptr, 0);
    if (error != DISP_CHANGE_SUCCESSFUL)
    {
        LOG_DEBUG("Error code: {}, ChangeDisplaySetting failed.", error);
    }

Exit:
    return(error);
}
