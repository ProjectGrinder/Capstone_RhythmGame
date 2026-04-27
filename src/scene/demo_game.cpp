#include "scene.h"

Scene::DemoGame Scene::DemoGame::instance()
{
    static DemoGame instance;
    return (instance);
}

Scene::DemoGame::ResourceManager Scene::DemoGame::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoGame Scene.");
    return ResourceManager();
}