#include "scene.h"
#include "system.h"

Scene::DemoRender Scene::DemoRender::instance()
{
    static DemoRender instance;
    return (instance);
}

Scene::DemoRender::TaskManager Scene::DemoRender::init()
{
    InputAttributeDescription rainbow_vs_input_attributes[] = {
        InputAttributeDescription{const_cast<char *>("Pos"), InputType::R32G32B32_FLOAT, 0},
        InputAttributeDescription{const_cast<char *>("Color"), InputType::R32G32B32A32_FLOAT, 12},
    };

    InputAttributeDescription rainbow_ps_input_attributes[] = {
        InputAttributeDescription{const_cast<char *>("SV_POSITION"), InputType::R32G32B32A32_FLOAT, 0},
        InputAttributeDescription{const_cast<char *>("Color"), InputType::R32G32B32A32_FLOAT, 16}
    };

    auto tm = TaskManager{};
    tm.create_entity(Game::Render::Camera2D{});
    tm.create_entity(
            Game::Render::Triangle{{{0,0}, {0.5, 0.5}, {1, 1}},0, 0},
            Game::Render::Material{
                .vert_shader = const_cast<char *>("shaders/vs/rainbow.cso"),
                .vert_shader_input_attributes = rainbow_vs_input_attributes,
                .pixel_shader = const_cast<char *>("shaders/ps/rainbow.cso"),
                .pixel_shader_input_attributes = rainbow_ps_input_attributes,
                .visible = true,
                .render_prior = 0,
                .color = {1,1,1,1}
            },
            Game::Render::IntentHandle{});
    tm.run_all();
    return (tm);
}

std::vector<Scene::DemoRender::ComponentTuple> Scene::DemoRender::exit()
{
    LOG_INFO("Exiting Demo Render Scene.");
    return {};
}
