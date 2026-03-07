#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
    #endif
    typedef void *Dx11AdapterHandler;
    typedef void *DirectXHandler;

    HRESULT dx11_adapter_init(Dx11AdapterHandler *handler, const DirectXHandler *directx);
    void dx11_adapter_cleanup(Dx11AdapterHandler *api);
#ifdef __cplusplus
}
#endif