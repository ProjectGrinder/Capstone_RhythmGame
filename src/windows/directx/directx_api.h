#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void *DirectxHandler;
    typedef struct DirectxConfig DirectxConfig;

    struct DirectxConfig
    {
        HWND window_handler;
        UINT width;
        UINT height;
        int is_window;
    };

    HRESULT directx_manager_init(_Out_ DirectxHandler *api, _In_ const DirectxConfig *config);
    void directx_manager_clean_up(_In_ const DirectxHandler *manager);

#ifdef __cplusplus
}
#endif

