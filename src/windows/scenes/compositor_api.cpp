#include "compositor_api.h"
#include "intent_api.h"

#include "system.h"

HRESULT compositor_init(CompositorHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::Render::Compositor *manager = nullptr;
    void *manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(System::Render::Compositor));
    if (manager_alloc == nullptr)
    {
        error = HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
        goto exit;
    }

    manager = new (manager_alloc) System::Render::Compositor();

    *handler = manager;
    exit:
        return (error);
}

void compositor_compose(IntentStorageHandler *intent_storage, CompositorHandler *compositor)
{
    if (intent_storage == nullptr)
    {
        return;
    }

    if (compositor == nullptr)
    {
        return;
    }

    System::Render::Compositor::compose(System::Render::IntentStorage::get_intents(), System::Render::IntentStorage::get_camera());
}

void compositor_cleanup(CompositorHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::Compositor *>(*api);
    manager->~Compositor();

    HeapFree(GetProcessHeap(), 0, manager);
    *api = nullptr;
}
