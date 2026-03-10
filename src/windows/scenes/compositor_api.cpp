#include "compositor_api.h"
#include "intent_api.h"

#include "system.h"

HRESULT compositor_init(CompositorHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::Render::Compositor *manager = nullptr;

    manager = new System::Render::Compositor();

    *handler = manager;
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

    System::Render::Compositor::compose(
            System::Render::IntentStorage::get_intents(), System::Render::IntentStorage::get_camera());
}

void compositor_cleanup(CompositorHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::Compositor *>(*api);
    delete manager;

    *api = nullptr;
}
