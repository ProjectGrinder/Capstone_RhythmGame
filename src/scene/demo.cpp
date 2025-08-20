#include "scene.h"

Scene::Demo Scene::Demo::instance()
{
    static Demo instance;
    return instance;
}

void Scene::Demo::test()
{
    test_component val;
    val.number = 1;
    LOG_DEBUG("Info: test call with val = {}", val.number);
}

Scene::Demo::TaskManager Scene::Demo::Init()
{
    ResourceManager resource_manager;
    return TaskManager(std::move(resource_manager));
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    return {};
}