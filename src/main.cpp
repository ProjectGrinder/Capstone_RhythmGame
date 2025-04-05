#include <Windows.h>
#include <thread>

#include "os.h"
#include "input_handler.h"

int WINAPI WinMain(
    _In_        HINSTANCE   hInstance,
    _In_opt_    HINSTANCE   hPrevInstance,
    _In_        char        *lpCmdLine,
    _In_        int         nShowCmd
)
{
    int error = ERROR_SUCCESS;

    using System::OS;
    using System::InputHandler;

    /* Initialize */

    /* Starting InputHandler */
    error = InputHandler::run();

    if (error == ERROR_SUCCESS)
    {
        /* Starting OS Process */
        error = OS::run();
    }

    /* Cleanup (Make sure this were run) */

    return(error);
}