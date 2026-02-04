#include "intent_api.h"
#include "system.h"

HRESULT intent_storage_init(IntentStorageHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::RenderStorage *manager = nullptr;
    void *manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(System::RenderStorage));
    if (manager_alloc == nullptr)
    {
        error = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        goto exit;
    }

    manager = new (manager_alloc) System::RenderStorage();

    *handler = manager;
exit:
    return (error);
}

void intent_storage_cleanup(IntentStorageHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::RenderStorage *>(*api);
    manager->~RenderStorage();

    HeapFree(GetProcessHeap(), 0, manager);
    *api = nullptr;
}
