#include "scene.h"
#include "system.h"

Scene::DemoRender Scene::DemoRender::instance()
{
    static DemoRender instance;
    return (instance);
}

std::shared_ptr<Scene::DemoRender::TaskManager> Scene::DemoRender::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    /*
    InputAttributeDescription rainbow_vs_input_attributes[] = {
            InputAttributeDescription{"Pos", InputType::R32G32B32_FLOAT, 0},
            InputAttributeDescription{"Color", InputType::R32G32B32A32_FLOAT, 12},
    };

    InputAttributeDescription rainbow_ps_input_attributes[] = {
            InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
            InputAttributeDescription{"Color", InputType::R32G32B32A32_FLOAT, 16}};

    auto rainbow_vs = load_vertex_shader("shaders/vs/rainbow.cso", "rainbow_vs", rainbow_vs_input_attributes, 2);

    auto rainbow_ps = load_pixel_shader("shaders/ps/rainbow.cso", "rainbow_ps", rainbow_ps_input_attributes, 2);
    */
    InputAttributeDescription sprite_vs_input_attributes[] = {
            InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
            InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
            InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

    InputAttributeDescription sprite_ps_input_attributes[] = {
            InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
            InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
            InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};

    auto sprite_vs = load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 2);

    auto sprite_ps = load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 2);

    auto sp = load_sprite("img/somebodyIusedToKnow.dds", "somebody", 512, 512);
    /*
    for (float i = 0; i < 100; ++i)
    {
        tm->create_entity(
                Game::Render::Triangle{
                        {{{-0.5, 0, 0}, {1, 0, 0, 1}}, {{0, 0.5, 0}, {0, 1, 0, 1}}, {{0.5, 0, 0}, {0, 0, 1, 1}}}, 0, 0},
                Game::Render::Material(rainbow_vs, rainbow_ps),
                Game::Render::Transform{Math::Point{{i * 0.1f, i * 0.1f, 0}, {0, 0, 0, 0}}, 0, 0, 0});
    }
    */


    tm->create_entity(
            Game::Physics::Rotation{0, 0, 0.002f},
            Game::Render::Sprite{
                    .sp = sp,
                    .pos = {{-256, 256, 0}, {256, 256, 0}, {256, -256, 0}, {-256, -256, 0}},
                    .layer = 1,
                    .u0 = 0.5f,
                    .v0 = 0.5f,
                    .u1 = 1.0f,
                    .v1 = 1.0f},
            Game::Render::Material(sprite_vs, sprite_ps),
            Game::Render::Transform{Math::Point{-300, 0, 0}, 0, 0, 0});

    auto sp2 = load_sprite("img/test.dds", "test", 500, 500);

    tm->create_entity(
            Game::Physics::Rotation{0, 0, 0.002f},
            Game::Render::Sprite{
                    .sp = sp2, .pos = {{-256, 256, 0}, {256, 256, 0}, {256, -256, 0}, {-256, -256, 0}}, .layer = 1},
            Game::Render::Material(sprite_vs, sprite_ps),
            Game::Render::Transform{Math::Point{300, 0, 0}, 0, 0, 0});

    auto sp3 = load_sprite("img/bg.dds", "bg", 1920, 1080);

    tm->create_entity(
            Game::Render::Sprite{
                    .sp = sp3, .pos = {{-640, 360, 0}, {640, 360, 0}, {640, -360, 0}, {-640, -360, 0}}, .layer = 1},
            Game::Render::Material(sprite_vs, sprite_ps),
            Game::Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});

    auto font = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

    tm->create_entity(
            Game::Test::FpsCounter{},
            Game::Render::Text{.font = font, .text = "0"},
            Game::Render::Material(sprite_vs, sprite_ps),
            Game::Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});

    return (tm);
}

std::vector<Scene::DemoRender::ComponentTuple> Scene::DemoRender::exit()
{
    LOG_INFO("Exiting Demo Render Scene.");
    return {};
}
