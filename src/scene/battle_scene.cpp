#include "scene.h"
#include "utils.h"

Scene::BattleScene Scene::BattleScene::instance()
{
    static BattleScene instance;
    return instance;
}

std::shared_ptr<Scene::BattleScene::TaskManager> Scene::BattleScene::init()
{
    LOG_INFO("Initialization called");
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Battle::BattleState());
    tm->create_entity(Game::BulletHell::Input());
    tm->create_entity(Game::Rhythm::KeyInput());
    tm->run_all();
    return (tm);
}

std::vector<Scene::BattleScene::ComponentTuple> Scene::BattleScene::exit()
{
    LOG_INFO("Exiting Battle Scene.");
    return {};
}
