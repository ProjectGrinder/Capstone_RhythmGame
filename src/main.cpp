#include <Windows.h>

#include "system.h"

int WINAPI WinMain(
    _In_        [[maybe_unused]]HINSTANCE   hInstance,
    _In_opt_    [[maybe_unused]]HINSTANCE   hPrevInstance,
    _In_        [[maybe_unused]]char        *lpCmdLine,
    _In_        [[maybe_unused]]int         nShowCmd
)
{
    uint32_t error = ERROR_SUCCESS;

    using System::OS;

    /* Initialize */

    if (error == ERROR_SUCCESS)
    {
        /* Starting OS Process */
        error = OS::run();
    }

    /* Cleanup (Make sure this were run) */

    return((int)error);
}