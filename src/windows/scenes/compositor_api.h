#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void *CompositorHandler;
    typedef void *IntentStorageHandler;

    HRESULT compositor_init(CompositorHandler *api);
    void compositor_cleanup(CompositorHandler *api);
    void compositor_compose(IntentStorageHandler *intent_storage, CompositorHandler *compositor, int frame_to_draw);
#ifdef __cplusplus
}
#endif
