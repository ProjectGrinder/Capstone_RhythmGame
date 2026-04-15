#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

Scene::DemoRhythm::ResourceManager Scene::DemoRhythm::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return ResourceManager();
}
