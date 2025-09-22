#include "scene.h"

Scene::Demo Scene::Demo::instance()
{
    static Demo instance;
    return instance;
}

void Scene::Demo::test()
{
    Game::Rotation val{1};
    LOG_DEBUG("Info: test call with val = {}", val.angle);
}

Scene::Demo::TaskManager Scene::Demo::Init()
{
    auto tm = TaskManager{};
    tm.create_entity(Game::Rotation{0}, Game::AngularVelocity{1});
    tm.run_all();
    return tm;
}

std::vector<Scene::Demo::ComponentTuple> Scene::Demo::Exit()
{
    return {};
}