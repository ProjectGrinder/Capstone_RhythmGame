#include "../../include/game/utils/DSL/bullethell/bullet_script.h"
#include "scene.h"
#include "system.h"

void init_graphics(const std::shared_ptr<Scene::DemoBulletHell::TaskManager>& tm)
{
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

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

    load_sprite("img/Default_Shot.dds", "bullet_sprite", 512, 512);
    load_sprite("img/test.dds", "test", 500, 500);
    load_sprite("img/BH_Player_Sprite.dds", "BH_Player_Sprite", 800, 1500);
    load_sprite("img/Hitbox.dds", "Hitbox", 12, 12);

    load_sprite("img/Square.dds", "Square", 64, 64);
    load_sprite("img/Square64px.dds", "Square64px", 64, 64);
    load_sprite("img/ring16px.dds", "ring16px", 72, 72);
}

Game::Render::AnimationDataRegistry Scene::init_anim_data()
{
    using namespace Game::Render;
    AnimationDataRegistry anim_datas{
        {
            {"Player_Idle_Front",{{0.25f,0.167f},0,3,10}},
            {"Player_Idle_Left",{{0.25f, 0.167f}, 1, 3, 10}},
            {"Player_Idle_Back",{{0.25f, 0.167f}, 2, 3, 10}},
            {"Player_Move_Front",{{0.25f,0.167f},3,4,10}},
            {"Player_Move_Left",{{0.25f,0.167f},4,4,10}},
            {"Player_Move_Back",{{0.25f,0.167f},5,4,10}},
            {"Boss_Idle",{{0.125f,0.33f},0,4,9}},
            {"Boss_Cast1",{{0.125f, 0.33f}, 1, 7, 9, true, 3}},
            {"Boss_Cast2",{{0.125f, 0.33f}, 2, 8, 9, false}},
        }
    };
    return anim_datas;
}

Game::Render::AnimationSequence Scene::init_boss_anim_seq()
{
    using namespace Game::Render;
    const float time_per_beat = 60000.f/ 134;
    auto TTB = [&time_per_beat](const float beat)->int { return (int)std::round(beat * time_per_beat)+3000; };
    AnimationSequence anim_seq{{
        // I don't understand why.
        {TTB(0), "Boss_Idle"},
    {TTB(3), "Boss_Cast1"},
    {TTB(5.5f), "Boss_Idle"},
    {TTB(83), "Boss_Cast1"},
    {TTB(87), "Boss_Idle"},
    {TTB(89), "Boss_Cast2"},
    {TTB(92), "Boss_Idle"},
    {TTB(105), "Boss_Cast1"},
    {TTB(146), "Boss_Cast2"},
    {TTB(150), "Boss_Idle"},
    {TTB(164), "Boss_Cast1"},
    {TTB(170), "Boss_Idle"},
    {TTB(172), "Boss_Cast2"},
    {TTB(175), "Boss_Cast1"},
    }};
    return anim_seq;
}

Scene::DemoBulletHell Scene::DemoBulletHell::instance()
{
    static DemoBulletHell instance;
    return (instance);
}

std::shared_ptr<Scene::DemoBulletHell::TaskManager> Scene::DemoBulletHell::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    init_graphics(tm);

    Game::BulletHell::BulletScript script{"dsl/ShotData.th0","dsl/01-B.th0"};

    // Create and configure BattleState
    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Render::AnimationDataRegistry,
    Game::Audio::SoundRegistry>
    (
        Game::Battle::BattleState(100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(10),
        std::move(script.bullet_registry),
        std::move(script.bullet_loader),
        std::move(script.pattern_container),
        init_anim_data(),
        Game::Audio::init_sounds(0)
        );

    // InputManager
    tm->create_entity<Game::Input>(Game::Input());


    auto hit_sound = load_audio("audio/fishdam1", "player_hit");
    AudioCache *out = nullptr;
    load_audio_if_not_exist((AssetsRecord *) hit_sound, &out);
    tm->create_entity<Game::BulletHell::Player,
    Game::Render::Transform,
    Rotation,
    Velocity,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material, Game::Render::Animator, Game::Render::Animation_Controller>(
        {}, Game::Render::Transform(0,-240), {}, {}, {},
        Game::Physics::CircularCollider(12),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 1,
            .u0 = 0.f, .v0 = 0.f, .u1 = 200.f/800.f, .v1 = 250.f/1500.f},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
        Game::Render::Animator{"Player_Idle_Fronts"}, Game::Render::Animation_Controller()
    );

    tm->create_entity<
            Game::BulletHell::PlayerHitbox,
            Game::Render::Transform,
            Game::Render::Sprite,
            Game::Render::Material>(
            Game::BulletHell::PlayerHitbox(7.5f),
            Game::Render::Transform(0, -240),
            Game::Render::Sprite{
                    .sp = get_assets_record_ptr(get_assets_id("Hitbox")),
                    .pos = {{-12, 12, 0}, {12, 12, 0}, {12, -12, 0}, {-12, -12, 0}},
                    .color = {1, 1, 1, 0},
                    .layer = 10},
            Game::Render::Material{
                    get_assets_record_ptr(get_assets_id("sprite_vs")),
                    get_assets_record_ptr(get_assets_id("sprite_ps"))});

    tm->create_entity<Game::Render::Transform,
    Game::Render::Sprite, Game::Render::Material, Game::Render::Animator, Game::Render::Animation_Controller, Game::Render::AnimationSequence, Game::Battle::BattleObject>(
        Game::Render::Transform(0,-240),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 1,
            .u0 = 0.f, .v0 = 0.f, .u1 = 200.f/800.f, .v1 = 250.f/1500.f},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
        Game::Render::Animator{"Boss_Idle"}, Game::Render::Animation_Controller(true), init_boss_anim_seq(), {Game::Battle::RHYTHM}
    );

    const auto font = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");
    tm->create_entity(
           Game::Test::FpsCounter{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-600, 300, 0}, 0, 0, 0});
    tm->create_entity(
           Game::Test::BulletCounter{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{400, 300, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Test::LifeText{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-600, 200, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Test::GrazeText{},
           Game::Render::Text{.font = font, .text = "0"},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{400, 200, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HPBarMax},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square64px")), .pos = {{-510, 12, 0}, {510, 12, 0}, {510, -12, 0}, {-510, -12, 0}}, .layer = 6},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HpBar},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-500, 10, 0}, {500, 10, 0}, {500, -10, 0}, {-500, -10, 0}}, .color = {0.2f,1,0.2f}, .layer = 5},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});
    tm->run_all();
    return (tm);
}

Scene::DemoBulletHell::ResourceManager Scene::DemoBulletHell::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return ResourceManager();
}