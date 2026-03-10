#include "intent_api.h"
#include "system.h"

HRESULT intent_storage_init(IntentStorageHandler *handler)
{
    HRESULT error = HRESULT_FROM_WIN32(ERROR_SUCCESS);
    System::Render::IntentStorage *manager = nullptr;

    manager = new System::Render::IntentStorage();

    *handler = manager;
    return (error);
}

void intent_storage_cleanup(IntentStorageHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<System::Render::IntentStorage *>(*api);

    delete manager;
    *api = nullptr;
}
