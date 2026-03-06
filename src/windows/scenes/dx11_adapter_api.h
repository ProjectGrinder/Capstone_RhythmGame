#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
    #endif
    typedef void *Dx11AdapterHandler;

    HRESULT dx11_adapter_init(Dx11AdapterHandler *api);
    void dx11_adapter_cleanup(Dx11AdapterHandler *api);
#ifdef __cplusplus
}
#endif