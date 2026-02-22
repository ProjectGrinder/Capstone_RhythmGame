#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void *IntentStorageHandler;

    HRESULT intent_storage_init(IntentStorageHandler *api);
    void intent_storage_cleanup(IntentStorageHandler *api);
#ifdef __cplusplus
}
#endif
