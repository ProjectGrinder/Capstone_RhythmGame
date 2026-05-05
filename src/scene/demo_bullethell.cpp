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

    load_sprite("img/Square.dds", "Square", 20, 20);
    load_sprite("img/Square20px.dds", "Square20px", 20, 20);
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

Game::Battle::BulletRegistry Scene::init_bullet_graphic()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletRegistry test{};
    read_bullet_data_from_file("ShotData.txt", test);
    return {test};
}

Game::Battle::PatternContainer Scene::create_pattern_container()
{
    using namespace Game::Battle;
    const std::vector<PatternStep> demo_step = {
        PatternStep(3000, OP_ADD, 2, 15),  // 3s Rot+15
        PatternStep(3000, OP_ADD, 2, -15), // 3s Rot-15
        PatternStep(3000, OP_SET, 1, 3),   // 3s Vel=3
};
    const std::vector<PatternSequence> demo_pattern = {
        PatternSequence(false),
        PatternSequence(false, 0, 2),
        PatternSequence(false, 1, 2),
        PatternSequence(true, 0, 2),
        PatternSequence(true, 1, 2),
    };
    auto demo_pattern_container = PatternContainer(demo_step,demo_pattern);
    return { PatternContainer(demo_pattern_container) };
}

Game::Battle::BulletLoader Scene::create_bullet_data()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;

        int left_padding = 2500;
        // Math::Point boss_pos = {0,240};

        BulletLoader loader;
        loader.CreateBullet(left_padding, BulletData(5, 5, 1,135, 100, 0));
        loader.CreateBullet(left_padding + 200, BulletData(5, 5,0.9f, 120 ,  100, 0));
        loader.CreateBullet(left_padding + 400, BulletData(5, 5,0.8f, 105, 100, 0));
        loader.CreateBullet(left_padding + 600, BulletData(5, 5,0.9f, 150, 100, 0));
        loader.CreateBullet(left_padding + 800, BulletData(5, 5, 0.8f, 165, 100, 0));

        loader.CreateBullet(left_padding + 3000, BulletData(10, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(20, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(40, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(50, 5, 1,135, 1, 0, 1));

        loader.CreateBullet(left_padding + 3000, BulletData(50, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(40, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(20, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(10, 5, 1,135, 2, 0, 1));

        loader.CreateBullet(left_padding + 7000, BulletData(25, 25, 0, 3));
        loader.CreateBullet(left_padding + 7500, BulletData(50, 50, 0, 3));
        loader.CreateBullet(left_padding + 8000, BulletData(75, 75, 0, 3));

        loader.CreateBullet(left_padding + 10000, BulletData(150, 25, 0, 3));
        loader.CreateBullet(left_padding + 10500, BulletData(125, 50, 0, 3));
        loader.CreateBullet(left_padding + 11000, BulletData(100, 75, 0, 3));

        loader.CreateBullet(left_padding + 12000, BulletData(25, 25, 150, 4));
        loader.CreateBullet(left_padding + 12500, BulletData(50, 50, 150, 4));
        loader.CreateBullet(left_padding + 13000, BulletData(75, 75, 150, 4));

        loader.CreateBullet(left_padding + 14000, BulletData(150, 25, 150, 4));
        loader.CreateBullet(left_padding + 14500, BulletData(125, 50, 150, 4));
        loader.CreateBullet(left_padding + 15000, BulletData(100, 75, 150, 4));
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
            loader.CreateBullet(1000 + i*100, BulletData(0, 0, 100, (static_cast<float>(j) *45) + 6 * i, 50,-45.f, 0, (i*8 + j)%152));
        }
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_data_boom_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<20;i++)
    {
        for (int j=0;j<4;j++)
        {
            loader.CreateBullet(1000 + i*3000 + j*100, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
        }
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_data_laser_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;
    for (int i=0;i<20;i++)
    {
        loader.CreateBullet(1000 + i*3000, BulletData(rand_float(-500,500), -300, 0, rand_float(-135,-45), 1000, 176));
        loader.CreateBullet(1000 + i*3000 + 250, BulletData(-500, rand_float(-300,300), 0, rand_float(-45,45), 1000, 177));
        loader.CreateBullet(1000 + i*3000 + 500, BulletData(rand_float(-500,500), 300, 0, rand_float(45,135), 0, 1000, 178));
        loader.CreateBullet(1000 + i*3000 + 750, BulletData(500, rand_float(-300,300), 0, rand_float(-225,-135), 1000, 179));
    }

    return loader;
}

Game::Battle::BulletLoader Scene::create_bullet_collision_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;

    for (int i=0;i<500;i++)
    {
        for (int j=0;j<8;j++)
        {
            loader.CreateBullet(1000 + i*250 + j*50, BulletData(0, 0, 100, (static_cast<float>(j) *45) + 6 * i, 50,-45.f, 0, (i*8 + j)%158));
        }
    }

    // for (int i=0;i<8;i++)
    // {
    //     for (int j=0;j<4;j++)
    //     {
    //         loader.CreateBullet(127000 + i*2000 + j*100, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, rand_int(169,177)));
    //     }
    // }

    // for (int i=0;i<10;i++)
    // {
    //     loader.CreateBullet(150000 + i*3000, BulletData(rand_float(-500,500), -300, 0, rand_float(-135,-45), 1000, 159));
    //     loader.CreateBullet(150000 + i*3000 + 250, BulletData(-500, rand_float(-300,300), 0, rand_float(-45,45), 1000, 161));
    //     loader.CreateBullet(150000 + i*3000 + 500, BulletData(rand_float(-500,500), 300, 0, rand_float(45,135), 0, 1000, 162));
    //     loader.CreateBullet(150000 + i*3000 + 750, BulletData(500, rand_float(-300,300), 0, rand_float(-225,-135), 1000, 163));
    // }

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
        init_bullet_graphic(),
        create_bullet_collision_test(),
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
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material, Game::Render::Animator>(
        {}, Game::Render::Transform(0,-240), {}, {}, {},{},
        Game::Physics::CircularCollider(24),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 1,
            .u0 = 0.f, .v0 = 0.f, .u1 = 200.f/800.f, .v1 = 250.f/1500.f},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
        Game::Render::Animator{0}
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
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square20px")), .pos = {{-55, 12.5, 0}, {55, 12.5, 0}, {55, -12.5, 0}, {-55, -12.5, 0}}, .layer = 6},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HpBar},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-50, 10, 0}, {50, 10, 0}, {50, -10, 0}, {-50, -10, 0}}, .color = {0.2f,1,0.2f}, .layer = 5},
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