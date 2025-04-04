#include <Windows.h>

int __stdcall WinMain(
    _In_        HINSTANCE   hInstance,
    _In_opt_    HINSTANCE   hPrevInstance,
    _In_        char        *lpCmdLine,
    _In_        int         nShowCmd
)
{
    OutputDebugStringW(L"Hello, World\n");
    return ERROR_SUCCESS;
}