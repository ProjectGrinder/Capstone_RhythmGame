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
    tm.create_entity(Game::Battle::BattleState());
    tm.create_entity(Game::BulletHell::Input());
    tm.create_entity(Game::Rhythm::KeyInput());
    tm.run_all();
    return (tm);
}

std::vector<Scene::BattleScene::ComponentTuple> Scene::BattleScene::Exit()
{
    LOG_INFO("Exiting Battle Scene.");
    return {};
}
