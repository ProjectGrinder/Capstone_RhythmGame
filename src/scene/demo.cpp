#include "scene.h"
#include "utils.h"

Scene::Demo Scene::Demo::instance()
{
    static Demo instance;
    return(instance);
}

void Scene::Demo::test()
{
    test_component val{1};
    LOG_DEBUG("Info: test call with val = {}", val.number);
}

Scene::Demo::TaskManager Scene::Demo::Init()
{
    LOG_DEBUG("Initialization called");
    auto tm = TaskManager{};
    tm.create_entity(test_component{1});
    tm.run_all();
    return(tm);
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    LOG_DEBUG("Exiting Demo Scene.");
    return {};
}