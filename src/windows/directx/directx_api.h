#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct DirectxAPI DirectxAPI;
    typedef struct DirectxConfig DirectxConfig;

    struct DirectxConfig
    {
        HWND window_handler;
        UINT width;
        UINT height;
        int is_window;
    };

    struct DirectxAPI
    {
        void *ptr;
    };

    HRESULT directx_manager_init(_Out_ DirectxAPI *api, _In_ const DirectxConfig *config);
    void directx_manager_clean_up(_In_ const DirectxAPI *manager);

#ifdef __cplusplus
}
#endif