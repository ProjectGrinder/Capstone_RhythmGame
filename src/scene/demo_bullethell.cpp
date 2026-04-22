#include "audio.h"
#include "scene.h"
#include "system.h"

void init_graphics(const std::shared_ptr<Scene::DemoBulletHell::TaskManager>& tm)
{
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    InputAttributeDescription sprite_vs_input_attributes[] = {
        InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12}};

    InputAttributeDescription sprite_ps_input_attributes[] = {
        InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16}};

    load_vertex_shader("shaders/vs/sprite.cso", "bullet_vs", sprite_vs_input_attributes, 2);

    load_pixel_shader("shaders/ps/sprite.cso", "bullet_ps", sprite_ps_input_attributes, 2);

    load_sprite("img/Default_Shot.dds", "bullet_sprite", 512, 512);
}

Game::Battle::BulletRegistry Scene::init_bullet_graphic()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    std::vector<BulletGraphicMap> maps = {
        BulletGraphicMap(ColliderData(CIRCLE, 5,5), GraphicData(0,12,17,29)),
        BulletGraphicMap(ColliderData(CIRCLE, 5,5), GraphicData(18,12,35,29)),
        BulletGraphicMap(ColliderData(CIRCLE, 5,5), GraphicData(108,12,125,29), {}, 20, 1, 100000),
        BulletGraphicMap(ColliderData(CIRCLE, 2,2), GraphicData(0,290,27,317) , SpecialBulletData(Booming, 5, 100), 5, 999, 3000),
        BulletGraphicMap(ColliderData(RECTANGLE, 2,2), GraphicData(1,431,25,459) , SpecialBulletData(Laser, 5, 100), 5, 999, 3000),
    };
    return {BulletRegistry(maps)};
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

        int left_padding = 5000;

        BulletLoader loader;
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 1,135, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 120 ,  100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.8f, 105, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 150, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 0.8f, 165, 100, 0));

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
    loader.CreateBullet(5000, BulletData(0, 0, 0,0, 1000, 3));
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

    init_graphics(tm);

    // Create and configure BattleState
    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Audio::SoundRegistry,
    Game::Rhythm::KeyInput, Game::BulletHell::Input>
    (
        Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(),
        init_bullet_graphic(),
        create_bullet_data(),
        create_pattern_container(),
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
    AngularVelocity, Game::Physics::CircularCollider>(
        {}, Game::Render::Transform(), {}, {}, {}, {}, {}
    );
    tm->run_all();
    return (tm);
}



std::vector<Scene::DemoBulletHell::ComponentTuple> Scene::DemoBulletHell::exit()
{
    LOG_INFO("Exiting DemoBulletHell Scene.");
    return {};
}