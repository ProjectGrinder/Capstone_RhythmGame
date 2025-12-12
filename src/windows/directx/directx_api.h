#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void *DirectXHandler;
    typedef void *RendererHandler;
    typedef void *VertexGeneratorHandler;
    typedef struct DirectXConfig DirectXConfig;

    struct DirectXConfig
    {
        HWND window_handler;
        UINT width;
        UINT height;
        BOOL is_window;
    };

    HRESULT directx_device_init(_Out_ DirectXHandler *api, _In_ const DirectXConfig *config);
    void directx_device_clean_up(_In_ DirectXHandler *manager);

    HRESULT directx_renderer_create(DirectXHandler *handler, RendererHandler *h_renderer);
    void directx_renderer_free(RendererHandler *h_renderer);

#ifdef __cplusplus
}
#endif
