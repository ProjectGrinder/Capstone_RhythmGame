#include "scene.h"

Scene::DemoGame Scene::DemoGame::instance()
{
    static DemoGame instance;
    return (instance);
}

std::vector<Scene::DemoGame::ComponentTuple> Scene::DemoGame::exit()
{
    LOG_INFO("Exiting DemoGame Scene.");
    return {};
}