#include "scene.h"
#include "system.h"

Scene::DemoRender Scene::DemoRender::instance()
{
    static DemoRender instance;
    return (instance);
}

std::shared_ptr<Scene::DemoRender::TaskManager> Scene::DemoRender::init()
{
    InputAttributeDescription rainbow_vs_input_attributes[] = {
            InputAttributeDescription{"Pos", InputType::R32G32B32_FLOAT, 0},
            InputAttributeDescription{"Color", InputType::R32G32B32A32_FLOAT, 12},
    };

    InputAttributeDescription rainbow_ps_input_attributes[] = {
            InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
            InputAttributeDescription{"Color", InputType::R32G32B32A32_FLOAT, 16}};

    auto vs = load_vertex_shader("shaders/vs/rainbow.cso", "rainbow_vs", rainbow_vs_input_attributes, 2);

    auto ps = load_pixel_shader("shaders/ps/rainbow.cso", "rainbow_ps", rainbow_ps_input_attributes, 2);

    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 16, .scaleY = 9, .rotation = 0});
    for (float i = 0; i < 10000; ++i)
    {
        tm->create_entity(
                Game::Render::Triangle{
                        {{{-0.5, 0, 0}, {1, 0, 0, 1}}, {{0, 0.5, 0}, {0, 1, 0, 1}}, {{0.5, 0, 0}, {0, 0, 1, 1}}}, 0, 0},
                Game::Render::Material(vs, ps),
                Game::Render::Transform{Math::Point{{i * 0.1f, i * 0.1f, 0}, {0, 0, 0, 0}}, 0, 0, 0});
    }
    tm->run_all();
    return (tm);
}

std::vector<Scene::DemoRender::ComponentTuple> Scene::DemoRender::exit()
{
    LOG_INFO("Exiting Demo Render Scene.");
    return {};
}
