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

std::shared_ptr<Scene::Demo::TaskManager> Scene::Demo::init()
{
    LOG_INFO("Initialization called");
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(test_component{1});
    tm->run_all();

    return (tm);
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting Demo Scene.");
    return {};
}
