#include "scene.h"
#include "system.h"

Scene::DemoRender Scene::DemoRender::instance()
{
    static DemoRender instance;
    return (instance);
}

Scene::DemoRender::TaskManager Scene::DemoRender::init()
{
    std::vector<InputAttributeDescription> rainbow_vs_input_attributes = {
            InputAttributeDescription{const_cast<char *>("Pos"), InputType::R32G32B32_FLOAT, 0},
            InputAttributeDescription{const_cast<char *>("Color"), InputType::R32G32B32A32_FLOAT, 12},
    };

    std::vector<InputAttributeDescription> rainbow_ps_input_attributes = {
            InputAttributeDescription{const_cast<char *>("SV_POSITION"), InputType::R32G32B32A32_FLOAT, 0},
            InputAttributeDescription{const_cast<char *>("Color"), InputType::R32G32B32A32_FLOAT, 16}};

    auto tm = TaskManager{};
    tm.create_entity(Game::Render::Camera2D{.scaleX = 16, .scaleY = 9, .rotation = 0});
    tm.create_entity(
            Game::Render::Triangle{
                    {{{-0.5, 0, 0}, {1, 0, 0, 1}}, {{0, 0.5, 0}, {0, 1, 0, 1}}, {{0.5, 0, 0}, {0, 0, 1, 1}}}, 0, 0},
            Game::Render::Material(
                    const_cast<char *>("shaders/vs/rainbow.cso"),
                    rainbow_vs_input_attributes,
                    2,
                    const_cast<char *>("shaders/ps/rainbow.cso"),
                    rainbow_ps_input_attributes,
                    2),
            Game::Render::Transform{
                    Math::Point{2, 2, 0, 0, 0, 0, 0}, 0, 0, 0},
            Game::Render::IntentHandle{});
    tm.run_all();
    return (tm);
}

std::vector<Scene::DemoRender::ComponentTuple> Scene::DemoRender::exit()
{
    LOG_INFO("Exiting Demo Render Scene.");
    return {};
}
