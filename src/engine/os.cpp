#include <string>
#include <format>

#include "os.h"

using System::OS;

OS OS::_instance;

OS& OS::instance()
{
    return(_instance);
}

int OS::run()
{
    int error = ERROR_SUCCESS;

    _instance._system_instance_handler = GetModuleHandleA(NULL);
    if (_instance._system_instance_handler == nullptr)
    {
        error = GetLastError();

        std::string buffer =
            std::format(
                "[ src/os.cpp | System::OS::run ] - GetModuleHandle failed with error code {}",
                error
            );

        OutputDebugStringA(buffer.c_str());
    }
#ifndef _TESTING
    if (error == ERROR_SUCCESS)
    {
        _instance._is_running = true;

        while (_instance._is_running)
        {
            /*  Polling Event  */

            /*  Upadate Game  */

            /*  Render  */

            /*
             *  Fix framerate delay
             *  TODO: Make this high precision
             */
            Sleep(1);
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
