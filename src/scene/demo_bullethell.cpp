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
            {{0.25f,0.167f},0,3,10},
            {{0.25f,0.167f},1,3,10},
            {{0.25f,0.167f},2,3,10},
            {{0.25f,0.167f},3,4,10},
            {{0.25f,0.167f},4,4,10},
            {{0.25f,0.167f},5,4,10},
        }
    };
    return anim_datas;
}

Game::Battle::PatternContainer Scene::create_pattern_container()
{
    using namespace Game::Battle;
    const std::vector<PatternStep> demo_step = {
        PatternStep(400, OP_SET, 0b1000, 300),
        PatternStep(0, OP_SET, 0b0100, -90),
        PatternStep(0, OP_SET, 0b0010, 0),
        PatternStep(1000, OP_SET, 0b1000, 300),
        PatternStep(0, OP_SET, 0b0010, 0),

};
    const std::vector<PatternSequence> demo_pattern = {
        PatternSequence(false, 0,1,2),
        PatternSequence(false, 0,1,2,3),

    };
    auto demo_pattern_container = PatternContainer(demo_step,demo_pattern);
    return { PatternContainer(demo_pattern_container) };
}

Game::Battle::BulletLoader Scene::create_bullet_data()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;

        BulletLoader loader;
        return loader;
    }

Game::Battle::BulletLoader Scene::create_bullet_data2()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<500;i++)
    {
        for (int j=0;j<8;j++)
        {
            loader.CreateBullet(1000 + i*100, BulletData(0, 0, 100, (static_cast<float>(j) *45) + 6 * (float)i, 50,-45.f,0, 5000, (i*8 + j)%152));
        }
    }

    return loader;
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

    // Create and configure BattleState
    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Render::AnimationDataRegistry,
    Game::Audio::SoundRegistry,
    Game::Rhythm::KeyInput, Game::BulletHell::Input>
    (
        Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(10),
        read_bullet_data_from_file("ShotData.txt"),
        create_bullet_data(),
        create_pattern_container(),
        init_anim_data(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input()
        );


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
        Game::Render::Animator{0}, Game::Render::Animation_Controller()
    );

    tm->create_entity<Game::BulletHell::PlayerHitbox, Game::Render::Transform, Game::Render::Sprite, Game::Render::Material>(
        Game::BulletHell::PlayerHitbox(7.5f), Game::Render::Transform(0,-240),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Hitbox")),
            .pos = {{-12, 12, 0}, {12, 12, 0}, {12, -12, 0}, {-12, -12, 0}}, .color = {1,1,1,0}, .layer = 10},
            Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))}
    );

    auto font = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");
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