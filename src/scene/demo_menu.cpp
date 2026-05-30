#include "scene.h"
#include "system.h"

Scene::DemoMenu Scene::DemoMenu::instance()
{
    static DemoMenu instance;
    return (instance);
}

void init_graphics(const std::shared_ptr<Scene::DemoMenu::TaskManager>& tm)
{
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    InputAttributeDescription sprite_vs_input_attributes[] = {
        InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

    InputAttributeDescription sprite_ps_input_attributes[] = {
        InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};

    load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 3);

    load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 3);

    load_sprite("img/piano.dds", "piano", 3840, 2160);
    load_sprite("img/music_verse.dds", "title", 1080, 1080);
    load_sprite("img/start.dds", "start", 1280, 720);
}

std::shared_ptr<Scene::DemoMenu::TaskManager> Scene::DemoMenu::init()
{
    auto tm = std::make_shared<TaskManager>();

    init_graphics(tm);

    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("piano")), .pos = {{-960, 540, 0}, {960, 540, 0}, {960, -540, 0}, {-960, -540, 0}}, .layer = 0},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});

    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("title")), .pos = {{-540, 540, 0}, {540, 540, 0}, {540, -540, 0}, {-540, -540, 0}}, .layer = 1},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, 350, 0}, 0, 0, 0});

    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("start")), .pos = {{-320, 180, 0}, {320, 180, 0}, {320, -180, 0}, {-320, -180, 0}}, .layer = 1},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, -360, 0}, 0, 0, 0});

    const auto font = load_font("fonts/Klub04TT-NoBG.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

    tm->create_entity<Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Text{.font = font, .text = "Made in: KAWA ENGINE", .layer = 1},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{-840, -480, 0}, 0, 0, 0});

    tm->create_entity<Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Text{.font = font, .text = "Developed by: Team Takayama", .layer = 1},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{480, -480, 0}, 0, 0, 0});

    return (tm);
}

Scene::DemoMenu::ResourceManager Scene::DemoMenu::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoMenu Scene.");
    return ResourceManager();
}