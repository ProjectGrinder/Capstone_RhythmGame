#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /* This scene will run on seperate threads */
    typedef struct SceneAPI SceneAPI;
    typedef void *SceneManagerHandler;

    HRESULT scene_manager_init(SceneManagerHandler *api);
    void scene_manager_update(SceneManagerHandler *api);
    void scene_manager_cleanup(SceneManagerHandler *api);
#ifdef __cplusplus
}
#endif
