#include "scene.h"

Scene::DemoOverworld Scene::DemoOverworld::instance()
{
    static DemoOverworld instance;
    return (instance);
}

std::vector<Scene::DemoOverworld::ComponentTuple> Scene::DemoOverworld::exit()
{
    LOG_INFO("Exiting DemoOverworld Scene.");
    return {};
}