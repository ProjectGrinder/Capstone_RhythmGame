#include "scene.h"

Scene::DemoBulletHell Scene::DemoBulletHell::instance()
{
    static DemoBulletHell instance;
    return (instance);
}

Scene::DemoBulletHell::ResourceManager Scene::DemoBulletHell::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return ResourceManager();
}