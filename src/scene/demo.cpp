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
    auto tm = TaskManager{};
    tm.run_all();
    return tm;
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    return {};
}