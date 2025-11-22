#include "directx_api.h"
#include "directx_manager.hpp"

HRESULT directx_manager_init(DirectxAPI *api, const DirectxConfig *config)
{
    HRESULT error = ERROR_SUCCESS;
    Windows::DirectxManager *manager;
    void* manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), 0, sizeof(Windows::DirectxManager));
    if (manager_alloc == nullptr)
    {
        error = ERROR_NOT_ENOUGH_MEMORY;
        goto exit;
    }

    manager = new(manager_alloc) Windows::DirectxManager();
    error = manager->init(reinterpret_cast<const Windows::DirectxConfig *>(config));

    api->ptr = manager;

exit:
    return(error);
}

void directx_manager_clean_up(const DirectxAPI *api)
{
    if (api == nullptr || api->ptr == nullptr)
        return;

    auto *manager = static_cast<Windows::DirectxManager *>(api->ptr);
    manager->~DirectxManager();

    HeapFree(GetProcessHeap(), 0, manager);
}
