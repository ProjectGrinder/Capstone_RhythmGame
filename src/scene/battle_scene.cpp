#include "scene.h"
#include "utils.h"

Scene::BattleScene Scene::BattleScene::instance()
{
    static BattleScene instance;
    return instance;
}

Scene::BattleScene::TaskManager Scene::BattleScene::Init()
{
    LOG_INFO("Initialization called");
    auto tm = TaskManager{};
    tm.create_entity(Game::Physics::Position{0, 0}, Game::Physics::Velocity{1,1});
    tm.run_all();
    return (tm);
}

std::vector<Scene::BattleScene::ComponentTuple> Scene::BattleScene::Exit()
{
    LOG_INFO("Exiting Demo Scene.");
    return {};
}
