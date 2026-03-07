#include "windows_functions.h"

#include "utils/windows_utils.h"

LARGE_INTEGER perf_frequency;

LRESULT CALLBACK
window_process(_In_ const HWND hwnd, _In_ const UINT msg, _In_ const WPARAM w_param, _In_ const LPARAM l_param)
{
    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        return (0);
    default:
        return (DefWindowProc(hwnd, msg, w_param, l_param));
    }
}

DWORD __vectorcall create_window(_In_ SystemInfo *system_info)
{
    HWND hwnd = {0};
    MONITORINFO monitor_info = {0};
    WNDCLASSEX window_settings = {0};
    DEVMODE devmode_screen_settings = {0};
    const DWORD style_flags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
    DWORD error = ERROR_SUCCESS;

    monitor_info.cbSize = sizeof(MONITORINFO);

    window_settings.cbSize = sizeof(WNDCLASSEXA);
    window_settings.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_settings.lpfnWndProc = window_process;
    window_settings.cbClsExtra = 0;
    window_settings.cbWndExtra = 0;
    window_settings.hInstance = system_info->instance_handler;
    window_settings.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    window_settings.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    window_settings.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_settings.hbrBackground = (HBRUSH) (GetStockObject(BLACK_BRUSH));
    window_settings.lpszMenuName = NULL;
    window_settings.lpszClassName = TO_STR(PROJECT_NAME);


    if (RegisterClassEx(&window_settings) == 0)
    {
        error = GetLastError();
        LOG_ERROR("RegisterClassEx failed, Code 0x%08lx", error);
        goto Exit;
    }

    hwnd = CreateWindowEx(
            0,
            window_settings.lpszClassName,
            window_settings.lpszClassName,
            style_flags,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            NULL,
            NULL,
            system_info->instance_handler,
            NULL);

    if (hwnd == NULL)
    {
        error = GetLastError();
        LOG_ERROR("CreateWindowEx failed, Code 0x%08lx", error);
        goto Exit;
    }

    /* We can do this because MonitorFromWindow Guarantee the return value */
    if (GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitor_info) == 0)
    {
        error = ERROR_MONITOR_NO_DESCRIPTOR;
        LOG_ERROR("GetMonitorInfo failed, Code 0x%08lx", error);
        goto Exit;
    }

    LONG monitor_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
    LONG monitor_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;

    LONG window_height = system_info->window.height;
    LONG window_width = system_info->window.width;

    if (window_width == 0)
    {
        window_width = monitor_width;
    }

    if (window_height == 0)
    {
        window_height = monitor_height;
    }

    if (window_width > monitor_width || window_height > monitor_height)
    {
        LOG_INFO("Either window width or window height is larger than monitor's value. Resetting");
        window_width = monitor_width;
        window_height = monitor_height;
    }

    LOG_INFO("Monitor size: %d x %d", monitor_width, monitor_height)
    LOG_INFO("Window size: %d x %d", window_width, window_height)

    switch (system_info->display_type)
    {
    case DT_FULLSCREEN:

        devmode_screen_settings.dmSize = sizeof(DEVMODEA);
        devmode_screen_settings.dmPelsWidth = window_width;
        devmode_screen_settings.dmPelsHeight = window_height;
        devmode_screen_settings.dmBitsPerPel = 32;
        devmode_screen_settings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        error = ChangeDisplaySettings(&devmode_screen_settings, CDS_FULLSCREEN);

        if (error != DISP_CHANGE_SUCCESSFUL)
        {
            LOG_ERROR("ChangeDisplaySettings failed, Code 0x%08lx", error);
            goto Exit;
        }

        if (SetWindowPos(
                    hwnd,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    window_width,
                    window_height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();
            LOG_ERROR("SetWindowPos failed, Code 0x%08lx", error);
            goto Exit;
        }

        break;

    case DT_BORDERLESS:

        if (SetWindowPos(
                    hwnd,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    monitor_width,
                    monitor_height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();
            LOG_ERROR("SetWindowPos failed, Code 0x%08lx", error);
            goto Exit;
        }

        break;

    case DT_WINDOW:
        if (SetWindowLongPtr(hwnd, GWL_STYLE, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)) == 0)
        {
            error = GetLastError();
            LOG_ERROR("SetWindowLongPtr failed, Code 0x%08lx", error);
            goto Exit;
        }

        if (SetWindowPos(
                    hwnd,
                    HWND_TOP,
                    (monitor_width - window_width) / 2,
                    (monitor_height - window_height) / 2,
                    window_width,
                    window_height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_ERROR("SetWindowPos failed, Code 0x%08lx", error);
            goto Exit;
        }

        break;

    default:

        error = ERROR_INVALID_PARAMETER;
        LOG_ERROR("Unknown display type, Code 0x%08lx", error);
        goto Exit;
    }

    ShowWindow(hwnd, SW_SHOW);

    system_info->monitor.height = monitor_height;
    system_info->monitor.width = monitor_width;

    system_info->window.height = window_height;
    system_info->window.width = window_width;

    system_info->window_handler = hwnd;

Exit:
    return (error);
}

void __vectorcall sleep_init(void)
{
    QueryPerformanceFrequency(&perf_frequency);
}

void __vectorcall process_system_message(_In_ SystemInfo *system_info, _In_ MSG *msg)
{
    while (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg->message == WM_QUIT)
            system_info->is_running = 0;
        DispatchMessage(msg);
    }
}

void __vectorcall sleep(_In_ const LONGLONG amount)
{
    LARGE_INTEGER start, end;

    QueryPerformanceCounter(&start);

    do
    {
        QueryPerformanceCounter(&end);
        Sleep(0);
    }
    while ((end.QuadPart - start.QuadPart) * 1000 / perf_frequency.QuadPart < amount);
}
