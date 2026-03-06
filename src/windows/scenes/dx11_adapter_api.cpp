#include "dx11_adapter_api.h"
#include "../directx/dx11_adapter.h"

HRESULT dx11_adapter_init(Dx11AdapterHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::Render::Dx11Adapter *manager = nullptr;
    void *manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(System::Render::Dx11Adapter));
    if (manager_alloc == nullptr)
    {
        error = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        goto exit;
    }

    manager = new (manager_alloc) System::Render::Dx11Adapter();

    *handler = manager;
    exit:
        return (error);
}

void dx11_adapter_cleanup(Dx11AdapterHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::Dx11Adapter *>(*api);
    manager->~Dx11Adapter();

    HeapFree(GetProcessHeap(), 0, manager);
    *api = nullptr;
}