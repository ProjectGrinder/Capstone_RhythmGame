#include "scene.h"

Scene::DemoBulletHell Scene::DemoBulletHell::instance()
{
    static DemoBulletHell instance;
    return (instance);
}

std::vector<Scene::DemoBulletHell::ComponentTuple> Scene::DemoBulletHell::Exit()
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return {};
}