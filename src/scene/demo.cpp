#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::Demo Scene::Demo::instance()
{
    static Demo instance;
    return (instance);
}

void Scene::Demo::test()
{
    test_component val{1};
    LOG_INFO("test call with val = {}", val.number);
}

Scene::Demo::TaskManager Scene::Demo::Init()
{
    LOG_INFO("Initialization called");
    auto tm = TaskManager{};
    tm.create_entity(test_component{1});
    tm.run_all();

    return (tm);
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    LOG_INFO("Exiting Demo Scene.");
    return {};
}
