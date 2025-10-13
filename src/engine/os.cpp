#include <assert.h>
#include "scene/scene_manager.h"
#include "system.h"
#include "utils.h"

#ifndef PROJECT_NAME
    #define PROJECT_NAME NO_NAME_PROJECT
#endif
#define STR(name) #name
#define TO_STR(name) STR(name)
#define TEST(name) #name

std::vector<char> read_shader(const std::string_view path)
{
    std::vector<char> data;
    uint32_t error = ERROR_SUCCESS;
    LARGE_INTEGER size = {0};
    DWORD bytesRead = 0;

    const HANDLE hFile = CreateFileA(
            path.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, CreateFileA failed", error);
        goto Exit;
    }

    if (GetFileSizeEx(hFile, &size) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, GetFileSizeEx failed", error);
        goto Error;
    }

    if (size.QuadPart == 0)
    {
        error = ERROR_EMPTY;
        LOG_DEBUG("File is empty");
        goto Error;
    }

    if (size.QuadPart > SIZE_MAX)
    {
        error = ERROR_FILE_TOO_LARGE;
        LOG_DEBUG("Error code: {}, File is too large", error);
        goto Error;
    }

    data.resize(static_cast<size_t>(size.QuadPart));

    if (ReadFile(hFile, data.data(), static_cast<DWORD>(size.QuadPart), &bytesRead, nullptr) == 0 ||
        bytesRead != static_cast<DWORD>(size.QuadPart))
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, ReadFile failed", error);
        goto Error;
    }
Error:
    if (CloseHandle(hFile) == 0)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, CloseHandle failed", error);
    }
Exit:
    return (data);
}

using System::OS;

OS::OS(): _window(1280, 720, 0, 0, nullptr, Renderer::WINDOW, false)
{
    this->_handler = GetModuleHandleA(nullptr);
    if (this->_handler == nullptr)
    {
        LOG_DEBUG("Error code: {}, GetModuleHandle failed.", GetLastError());
        throw std::runtime_error("Unable to get current instance handler.\nMore detail please check on logs");
    }

    if (timeBeginPeriod(1) != TIMERR_NOERROR)
    {
        LOG_DEBUG("timeBeginPeriod failed.");
        throw std::runtime_error("Unable to set time resolution.\nMore detail please check on logs");
    }
}

OS &OS::instance()
{
    static OS _instance;
    return (_instance);
}

uint32_t OS::run()
{
    uint32_t error = ERROR_SUCCESS;

    error = instance()._create_window();
    if (error != ERROR_SUCCESS)
        goto Exit;

    error = instance()._renderer.init(instance()._window);
    if (error != ERROR_SUCCESS)
        goto Exit;

    Scene::SceneManager::init(instance()._vertex_queue.get());

    instance()._run();

Exit:
    return (error);
}

bool OS::is_running()
{
    return (instance()._window.is_running);
}

void OS::stop()
{
    instance()._window.is_running = false;
}

uint32_t OS::set_system_precision(int32_t ms)
{
    if (ms <= 0)
    {
        Utils::print_debug(
                "engine/os.cpp",
                __FUNCTION__,
                "Error code: {}, Cannot set system time precision to value less or equal to 0.",
                ERROR_INVALID_PARAMETER);
        return (ERROR_INVALID_PARAMETER);
    }
    else
    {
        instance()._system_precision = static_cast<uint16_t>(ms);
        return (ERROR_SUCCESS);
    }
}

void OS::_sleep() const
{
    /* This function will always succeed after Win XP */
    LARGE_INTEGER start, end, frequency;

    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&start);

    do
    {
        QueryPerformanceCounter(&end);
        Sleep(0);
    }
    while ((end.QuadPart - start.QuadPart) * 1000 / frequency.QuadPart < this->_system_precision);
}

struct Vertex
{
    float position[3];
    float color[4];
};

void OS::_run()
{
    _window.is_running = true;

    while (_window.is_running)
    {
        /*  Polling Event  */
        _poll_event();
        /*  Update Game  */
        Scene::SceneManager::update();
        /*  Render  */

        _sleep();
    }
}

void OS::_poll_event() const
{
    MSG msg;
    while (PeekMessage(&msg, this->_window.window_handler, 0, 0, PM_REMOVE))
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            Input::set_key_down(static_cast<uint8_t>(msg.wParam));
            break;
        case WM_KEYUP:
            Input::set_key_up(static_cast<uint8_t>(msg.wParam));
            break;
        case WM_MOUSEMOVE:
            Input::set_mouse_position(static_cast<uint16_t>(msg.pt.x), static_cast<uint16_t>(msg.pt.y));
            break;
        default:
            break;
        }

        DispatchMessageA(&msg);
    }
}

uint16_t OS::get_system_precision()
{
    return (instance()._system_precision);
}

uint32_t OS::_create_window()
{
    MONITORINFO monitor_info = {0};
    WNDCLASSEXA window_settings = {0};
    DEVMODEA devmode_screen_settings = {0};

    constexpr DWORD style_flags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

    window_settings.cbSize = sizeof(WNDCLASSEXA);
    window_settings.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_settings.lpfnWndProc = window_process;
    window_settings.cbClsExtra = 0;
    window_settings.cbWndExtra = 0;
    window_settings.hInstance = this->_handler;
    window_settings.hIcon = LoadIconA(nullptr, IDI_WINLOGO);
    window_settings.hIconSm = LoadIconA(nullptr, IDI_WINLOGO);
    window_settings.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    window_settings.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
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

    this->_window.window_handler = CreateWindowExA(
            0,
            window_settings.lpszClassName,
            window_settings.lpszClassName,
            style_flags,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            nullptr,
            nullptr,
            this->_handler,
            nullptr);

    if (this->_window.window_handler == nullptr)
    {
        error = GetLastError();
        LOG_DEBUG("Error code: {}, Failed to create window.", error);
        goto Exit;
    }

    /* We can do this because MonitorFromWindow Guarantee the return value */
    if (GetMonitorInfoA(MonitorFromWindow(this->_window.window_handler, MONITOR_DEFAULTTOPRIMARY), &monitor_info) == 0)
    {
        error = ERROR_MONITOR_NO_DESCRIPTOR;

        LOG_DEBUG("Error code: {}, GetMonitorInfo failed.", error);
        goto Exit;
    }

    this->_window.monitor_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
    this->_window.monitor_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;

    if (this->_window.width == 0)
    {
        this->_window.width = this->_window.monitor_width;
    }

    if (this->_window.height == 0)
    {
        this->_window.height = this->_window.monitor_height;
    }

    if (this->_window.width > this->_window.monitor_width || this->_window.height > this->_window.monitor_height)
    {
        LOG_DEBUG("Info: Either window width or window height is larger than monitor's value. Resetting");

        this->_window.width = this->_window.monitor_width;
        this->_window.height = this->_window.monitor_height;
    }

    LOG_DEBUG("Info: Monitor: {}x{}", this->_window.monitor_width, this->_window.monitor_height);
    LOG_DEBUG("Info: Window: {}x{}", this->_window.width, this->_window.height);

    switch (this->_window.display_type)
    {
    case Renderer::FULLSCREEN:

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

        if (SetWindowPos(
                    this->_window.window_handler,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    this->_window.width,
                    this->_window.height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case Renderer::BORDERLESS:

        if (SetWindowPos(
                    this->_window.window_handler,
                    HWND_TOP,
                    monitor_info.rcMonitor.left,
                    monitor_info.rcMonitor.top,
                    this->_window.monitor_width,
                    this->_window.monitor_height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowPos failed.", error);
            goto Exit;
        }

        break;

    case Renderer::WINDOW:

        if (SetWindowLongPtrA(
                    this->_window.window_handler, GWL_STYLE, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)) == 0)
        {
            error = GetLastError();

            LOG_DEBUG("Error code: {}, SetWindowLongPtr failed.", error);
            goto Exit;
        }

        if (SetWindowPos(
                    this->_window.window_handler,
                    HWND_TOP,
                    (this->_window.monitor_width - this->_window.width) / 2,
                    (this->_window.monitor_height - this->_window.height) / 2,
                    this->_window.width,
                    this->_window.height,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED) == 0)
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

    ShowWindow(this->_window.window_handler, SW_SHOW);

Exit:
    return (error);
}

static LRESULT CALLBACK System::window_process(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        OS::stop();
        PostQuitMessage(0);
        return (0);
    default:
        return (DefWindowProc(hwnd, msg, w_param, l_param));
    }
}

uint32_t OS::clean_up()
{
    LOG_DEBUG("Info: Cleaning up...");

    instance()._vertex_queue.reset();

    if (timeEndPeriod(1) != TIMERR_NOERROR)
    {
        LOG_DEBUG("timeEndPeriod failed.");
    }

    return (ERROR_SUCCESS);
}
