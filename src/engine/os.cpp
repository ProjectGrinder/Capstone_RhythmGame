#include "system.h"
#include "utils.h"
#include "input.h"

using System::OS;

OS OS::_instance;

OS::OS()
{
    _instance._system_instance_handler = GetModuleHandleA(NULL);
    if (_instance._system_instance_handler == nullptr)
    {
        Utils::print_debug
        (
            "engien/os.cpp",
            "System::OS::OS",
            "Error code: {}, GetModuleHandle failed.",
            GetLastError()
        );
    }
}

OS& OS::instance()
{
    return(_instance);
}

uint32_t OS::run()
{
    uint32_t error;
    if (_instance._system_instance_handler == nullptr)
    {
        error = ERROR_INVALID_STATE;
        return(error);
    }

    _instance._window = std::make_unique<Window>(_instance._system_instance_handler);
    error = _instance._window->create_window();

#ifndef _TESTING
    if (error == ERROR_SUCCESS)
    {
        _instance._is_running = true;

        while (_instance._is_running)
        {
            /*  Polling Event  */
            OS::_poll_event();
            /*  Upadate Game  */

            /*  Render  */

            /*
             *  Fix framerate delay
             *  TODO: Make this high precision
             */
            OS::_sleep();
        }
    }
#endif
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

uint32_t OS::set_system_precision(int64_t ms)
{
    if (ms <= 0)
    {
        Utils::print_debug
        (
            "engine/os.cpp",
            "OS::set_system_precision",
            "Error code: {}, Cannot set system time precision to value less or equal to 0.",
            ERROR_INVALID_PARAMETER
        );
        return(ERROR_INVALID_PARAMETER);
    }
    else
    {
        _instance._system_precision = ms;
        return(ERROR_SUCCESS);
    }

    return(ERROR_INVALID_STATE);
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
            _instance._window->get_handle(),
            0,0, PM_REMOVE
        )
    )
    {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            if (msg.wParam == VK_ESCAPE)
            {
                stop();
            }
            Input::set_key_down(msg.wParam);
            break;
        case WM_KEYUP:
            Input::set_key_up(msg.wParam);
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
