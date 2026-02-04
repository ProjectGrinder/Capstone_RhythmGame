#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

std::vector<Scene::DemoRhythm::ComponentTuple> Scene::DemoRhythm::Exit()
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return {};
}
