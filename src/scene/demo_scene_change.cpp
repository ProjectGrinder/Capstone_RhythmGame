#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::DemoSceneChange Scene::DemoSceneChange::instance()
{
    static DemoSceneChange instance;
    return (instance);
}

void Scene::DemoSceneChange::test()
{
    test_component val{1};
    LOG_INFO("test call with val = {}", val.number);
}

std::shared_ptr<Scene::DemoSceneChange::TaskManager> Scene::DemoSceneChange::init()
{
    LOG_INFO("Initialization called");
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(test_component{1000});
    tm->run_all();

    return (tm);
}

std::shared_ptr<Scene::DemoSceneChange::TaskManager> Scene::DemoSceneChange::init(Scene::DemoSceneChange::ResourceManager &data)
{
    LOG_INFO("Initialization called (scene changed)");
    auto tm = std::make_shared<TaskManager>();

    auto pool = data.query<test_component>();

    for (auto [id, component]: pool)
    {
        tm->create_entity(test_component{component.number});
    }

    tm->run_all();

    return (tm);
}

Scene::DemoSceneChange::ResourceManager Scene::DemoSceneChange::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting Demo Scene.");
    return ResourceManager();
}
