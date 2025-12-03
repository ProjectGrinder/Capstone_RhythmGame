#include "scenes_api.h"
#include "scene.h"

HRESULT scene_manager_init(SceneManagerHandler *handler)
{
    HRESULT error = ERROR_SUCCESS;
    Scene::SceneManager *manager = nullptr;
    void *manager_alloc = nullptr;

    manager_alloc = HeapAlloc(GetProcessHeap(), 0, sizeof(Scene::SceneManager));
    if (manager_alloc == nullptr)
    {
        error = ERROR_NOT_ENOUGH_MEMORY;
        goto exit;
    }

    manager = new (manager_alloc) Scene::SceneManager();

    *handler = manager;

    manager->change_scene<Scene::Config::StartingScene>();
exit:
    return (error);
}

void scene_manager_update(SceneManagerHandler *handle)
{
    if (handle == nullptr)
    {
        return;
    }
    /* FATAL: DO NOT USE THIS IS FOR TESTING ONLY! */
    Scene::SceneManager::update();
}

void scene_manager_cleanup(const SceneManagerHandler *api)
{
    if (api == nullptr)
        return;

    auto *manager = static_cast<Scene::SceneManager *>(*api);
    manager->~SceneManager();

    HeapFree(GetProcessHeap(), 0, manager);
}
