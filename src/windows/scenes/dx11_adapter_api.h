#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void *Dx11AdapterHandler;
    typedef void *DirectXHandler;
    typedef void *CompositorHandler;

    HRESULT dx11_adapter_init(Dx11AdapterHandler *handler, DirectXHandler *directx);
    void dx11_adapter_convert(Dx11AdapterHandler *api, DirectXHandler *directx, CompositorHandler *compositor);
    void dx11_adapter_render(Dx11AdapterHandler *api, DirectXHandler *directx);
    void dx11_adapter_cleanup(Dx11AdapterHandler *api);
#ifdef __cplusplus
}
#endif

