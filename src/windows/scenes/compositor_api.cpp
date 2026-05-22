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

void compositor_compose(IntentStorageHandler *intent_storage, CompositorHandler *compositor, int frame_to_draw)
{
    if (intent_storage == nullptr || compositor == nullptr)
    {
        return;
    }

    size_t active_count = 0;
    const auto &intents = System::Render::IntentStorage::get_packed_for_render(frame_to_draw, active_count);

    System::Render::Compositor::compose(intents, active_count, System::Render::IntentStorage::get_camera());
}

void compositor_cleanup(CompositorHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::Compositor *>(*api);
    delete manager;

    *api = nullptr;
}
