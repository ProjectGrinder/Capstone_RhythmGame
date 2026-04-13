#include "scene.h"

Scene::DemoGame Scene::DemoGame::instance()
{
    static DemoGame instance;
    return (instance);
}

std::vector<Scene::DemoGame::ComponentTuple> Scene::DemoGame::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoGame Scene.");
    return {};
}