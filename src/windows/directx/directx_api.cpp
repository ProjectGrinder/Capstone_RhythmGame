#include "directx_api.h"
#include "device_resources.hpp"

HRESULT directx_device_init(DirectXHandler *api, const DirectXConfig *config)
{
    HRESULT error = S_OK;
    Windows::DeviceResources *device_resources;
    void *alloc = nullptr;

    alloc = HeapAlloc(GetProcessHeap(), 0, sizeof(*device_resources));
    if (alloc == nullptr)
    {
        error = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        goto exit;
    }

    device_resources = new (alloc) Windows::DeviceResources();

    error = device_resources->init(reinterpret_cast<const Windows::DirectXConfig *>(config));
    if (FAILED(error))
    {
        HeapFree(GetProcessHeap(), 0, device_resources);
        goto exit;
    }

    *api = device_resources;

exit:
    return (error);
}

void directx_device_clean_up(const DirectXHandler *api)
{
    if (api == nullptr || *api == nullptr)
        return;

    auto *manager = static_cast<Windows::DeviceResources *>(*api);
    manager->~DeviceResources();

    HeapFree(GetProcessHeap(), 0, manager);
    api = nullptr;
}
