#include "scene.h"
#include "system.h"
#include "utils.h"

Scene::DemoRender Scene::DemoRender::instance()
{
    static DemoRender instance;
    return (instance);
}

Scene::DemoRender::TaskManager Scene::DemoRender::init()
{
    auto tm = TaskManager{};
    tm.create_entity(Game::Render::Camera2D{});
    tm.create_entity(
            Game::Render::Sprite{const_cast<char *>("shaders/ps/rainbow.hlsl"), Game::Render::Rect(0, 0, 1, 1)},
            Game::Render::IntentHandle{});
    tm.run_all();
    return (tm);
}

std::vector<Scene::DemoRender::ComponentTuple> Scene::DemoRender::exit()
{
    LOG_INFO("Exiting Demo Render Scene.");
    return {};
}
