#include "intent_api.h"
#include "system.h"

HRESULT intent_storage_init(IntentStorageHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::Render::IntentStorage *manager = nullptr;
    void *manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(System::Render::IntentStorage));
    if (manager_alloc == nullptr)
    {
        error = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        goto exit;
    }

    manager = new (manager_alloc) System::Render::IntentStorage();

    *handler = manager;
exit:
    return (error);
}

void intent_storage_cleanup(IntentStorageHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::IntentStorage *>(*api);
    manager->~IntentStorage();

    HeapFree(GetProcessHeap(), 0, manager);
    *api = nullptr;
}
