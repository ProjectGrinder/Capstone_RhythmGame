#include <Windows.h>

#include "os.h"

int WINAPI WinMain(
    _In_        HINSTANCE   hInstance,
    _In_opt_    HINSTANCE   hPrevInstance,
    _In_        char        *lpCmdLine,
    _In_        int         nShowCmd
)
{
    int error = ERROR_SUCCESS;

    using System::OS;

    /* Initialize */

    if (error == ERROR_SUCCESS)
    {
        /* Starting OS Process */
        error = OS::run();
    }

    /* Cleanup (Make sure this were run) */

    return(error);
}