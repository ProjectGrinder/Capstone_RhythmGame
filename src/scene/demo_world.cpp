#include "scene/demo_world.h"

void init_graphics(const std::shared_ptr<Scene::DemoWorld::TaskManager>& tm)
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

    load_sprite("img/test.dds", "test", 500, 500);

    load_sprite("img/bullethell/BH_Player_Sprite.dds", "BH_Player_Sprite", 800, 1500);

    load_sprite("img/world_map/tile1.dds", "PlatformTop", 64, 64);
    load_sprite("img/world_map/tile2.dds", "Platform", 64, 64);
    load_sprite("img/world_map/companion.dds", "Npc1", 564, 840);
    load_sprite("img/world_map/mayor.dds", "Npc2", 744, 784);
    load_sprite("img/world_map/level-node.dds", "LevelNode", 684, 700);
    load_sprite("img/world_map/BG3.dds", "Bg1", 5408, 2160);
    load_sprite("img/world_map/BG2.dds", "Bg2", 5408, 2160);
    load_sprite("img/world_map/BG1.dds", "Bg3", 5408, 2160);

    load_sprite("img/Square.dds", "Square", 64, 64);
    load_sprite("img/Square64px.dds", "Square64px", 64, 64);
    load_sprite("img/ring16px.dds", "ring16px", 72, 72);

    load_font("fonts/Klub04TT-NoBG.dds", "Klub04TT-NoBG", "fonts/Klub04TT-Normal.txt");
}

inline Game::Battle::LevelData create_level1_data()
{
    return Game::Battle::LevelData(
    "A World Without You",
    "Nakuya",
    134.00f,
    Game::Battle::BpmInfo({Game::Battle::BpmInfo::InfoPair(-3000, 134.00f)}),
        {
            Game::Battle::Difficulty(Game::Battle::LIGHT, 1, 10000,20),
            Game::Battle::Difficulty(Game::Battle::SPARK, 3, 10000,30),
            Game::Battle::Difficulty(Game::Battle::BLAZE, 5, 10000,40),
        },142000
    );
}

inline Game::Battle::LevelData create_level2_data()
{
    return Game::Battle::LevelData(
    "Strike Against The World!",
    "Pooh5821",
    170.00f,
    Game::Battle::BpmInfo({Game::Battle::BpmInfo::InfoPair(-3000, 170.00f)}),
{
            Game::Battle::Difficulty(Game::Battle::LIGHT, 2, 10000,20),
            Game::Battle::Difficulty(Game::Battle::SPARK, 4, 10000,30),
            Game::Battle::Difficulty(Game::Battle::BLAZE, 5, 10000,40),
        }, 139000
    );
}

Game::World::DialogueRegistry init_dialogue_registry()
{
    using namespace Game::World;
    std::vector<std::string> text_register = {
        "Hey.",
        "You're finally awake.",
        "Not sure what's going on?",
        "Just follow the music and you will be fine.",
        "Go on. Try interacting with that crystal.",
        "So you've come to see me!",
        "I'm just up here to enjoy the scenery",
        "I'll come down when you want to battle me.",
        "May the odds be in your favor."
    };
    return {DialogueRegistry(text_register)};
}

Game::World::EventRegister init_event_registry()
{
    using namespace Game::World;
    EventRegister event_sequences = {
        {LockInputEvent(0b100), DialogueEvent(0), DialogueEvent(1), DialogueEvent(2), DialogueEvent(3),
            DialogueEvent(4), UnlockInputEvent()},
        {LockInputEvent(0b100), LevelNodeEvent(0), UnlockInputEvent()},
        {LockInputEvent(0b100), LevelNodeEvent(1), UnlockInputEvent()},
        {LockInputEvent(0b100), DialogueEvent(5), DialogueEvent(6), DialogueEvent(7), DialogueEvent(8),
            UnlockInputEvent()}
    };
    return { EventRegister(event_sequences) };
}

Game::World::SceneRegistry init_scene_registry()
{
    using namespace Game::World;
    SceneRegistry scene_registry = {
        {
            // SceneObject(0,0,0.4f,0.4f,0, {Bg1}, {}),
            // SceneObject(0,0,0.4f,0.4f,0, {Bg2}, {}),
            // SceneObject(0,0,0.4f,0.4f,0, {Bg3}, {}),
            SceneObject(64*25,64*2,0.1f,0.1f, 1, 1, {LevelNode}, {}),
            SceneObject(64*59,64*0,0.1f,0.1f, 1, 2, {LevelNode}, {}),
            SceneObject(64*20,64*2.3f,0.1f,0.1f, 1, 0, {Npc1}, {}),
            SceneObject(64*47,64*4.2f,0.1f,0.1f, 1, 3, {Npc2}, {})
        }
    };
    for (int i=-15; i<15; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*-1,1,1,2, {PlatformTop}, {}));
        for (int j=-3; j>=-7; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=15; i<31; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),0,1,1,2, {PlatformTop}, {}));
        for (int j=-2; j>=-10; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=-17; i>-37; i-=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*7,1,1,2, {PlatformTop}, {}));
        for (int j=5; j>=-7; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=35; i<41; i++)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64,0.5f,0.5f,2, {PlatformTop}, {}));
    }
    for (int i=44; i<50; i++)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*2.5f,0.5f,0.5f,2, {PlatformTop}, {}));
    }
    scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*31),64*-1.5f,1,1,2, {PlatformTop}, {}));
    for (int j=-3; j>=-9; j-=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*31),64*j,1,1,2, {Platform}, {}));
    }
    for (int i=33; i<51; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*-3,1,1,2, {PlatformTop}, {}));
        for (int j=-5; j>=-9; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=51; i<65; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*-2,1,1,2, {PlatformTop}, {}));
        for (int j=-4; j>=-10; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=65; i<69; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*-1,1,1,2, {PlatformTop}, {}));
        for (int j=-3; j>=-9; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    for (int i=69; i<85; i+=2)
    {
        scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*4,1,1,2, {PlatformTop}, {}));
        for (int j=2; j>=-8; j-=2)
        {
            scene_registry.scene_objects.emplace_back(SceneObject(static_cast<float>(64*i),64*j,1,1,2, {Platform}, {}));
        }
    }
    return scene_registry;

}


Scene::DemoWorld Scene::DemoWorld::instance()
{
    static DemoWorld instance;
    return (instance);
}



std::shared_ptr<Scene::DemoWorld::TaskManager> Scene::DemoWorld::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {0,0}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});
    init_graphics(tm);

    // Create and configure BattleState
    tm->create_entity<Game::World::DialogueRegistry,
    Game::World::EventRegister,
    Game::World::SceneRegistry,
    Game::World::GlobalState,
    Game::World::PlayerStat,
    Game::Audio::SoundRegistry>
    (
        init_dialogue_registry(),
        init_event_registry(),
        init_scene_registry(),
        Game::World::GlobalState(),
        Game::World::PlayerStat(),
        Game::Audio::init_world_sounds()
        );

    constexpr int bg_width = 5408/2;
    constexpr int bg_height = 2160/2;

    tm->create_entity<
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform,
        Game::World::Background>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Bg1")), .pos = {{-bg_width,bg_height,0},{bg_width,bg_height,0},{bg_width,-bg_height,0},{-bg_width,-bg_height,0}},
        .layer = 0},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{300, 0, 0, 0, 0, 0.5f, 0.5f, 1},
        Game::World::Background());
    tm->create_entity<
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform,
        Game::World::Background>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Bg2")), .pos = {{-bg_width,bg_height,0},{bg_width,bg_height,0},{bg_width,-bg_height,0},{-bg_width,-bg_height,0}},
        .layer = 1},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{300, 0, 0, 0, 0, 0.5f, 0.5f, 1},
        Game::World::Background(0.1f));
    tm->create_entity<
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform,
        Game::World::Background>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Bg3")), .pos = {{-bg_width,bg_height,0},{bg_width,bg_height,0},{bg_width,-bg_height,0},{-bg_width,-bg_height,0}},
        .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{300, 0, 0, 0, 0, 0.5f, 0.5f, 1},
        Game::World::Background(0.1f));

    // SaveState
    tm->create_entity<Game::World::SaveState>(Game::World::SaveState());
    // InputManager
    tm->create_entity<Game::Input>(Game::Input());
    // LevelRegistry
    // TODO : Move this to init at the start of the game
    tm->create_entity<Game::World::LevelRegistry>({
        { create_level1_data(), create_level2_data()}
    });

    tm->create_entity<Game::World::Player, Rotation,
    Game::Render::Transform,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material>(
        {},{}, {0,100}, {}, Acceleration(0,0,1000,1000,-1000,-1000), {}, Game::Physics::CircularCollider(20.f,32.f),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 20,
            .u0 = 0.f, .v0 = 0.f, .u1 = 200.f/800.f, .v1 = 250.f/1500.f},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))}
    );
    tm->run_all();
    return (tm);
}

Scene::DemoWorld::ResourceManager Scene::DemoWorld::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    // Fuck me
    LOG_INFO("Exiting DemoGame Scene.");

    free_assets(get_assets_id("Platform"));
    free_assets(get_assets_id("PlatformTop"));
    free_assets(get_assets_id("Npc1"));
    free_assets(get_assets_id("Npc2"));
    free_assets(get_assets_id("LevelNode"));

    ResourceManager rm;
    // Selected Level
    if ( manager->get_rm()->query<Game::World::GlobalState>().begin() ==  manager->get_rm()->query<Game::World::GlobalState>().end()) LOG_ERROR("No global");
    const Game::World::GlobalState global = manager->get_rm()->query<Game::World::GlobalState>().front();
    const System::ECS::pid level_id = rm.reserve_process();
    System::ECS::ResourcePool<1000, Game::World::LevelRegistry> &level_registry = manager->get_rm()->query<Game::World::LevelRegistry>();
    if (level_registry.begin() == level_registry.end()) LOG_ERROR("Please dont happen");
    const Game::Battle::LevelData &level_data = level_registry.front().level_datas[global.level_selected];
    rm.add_resource(level_id, Game::Battle::LevelData{
        level_data.title,
        level_data.artist_name,
        level_data.main_bpm,
        level_data.bpm_info,
        level_data.difficulties,
        level_data.duration
    });

    const System::ECS::pid battle_id = rm.reserve_process();
    rm.add_resource(battle_id, Game::Battle::BattleState(100,level_data.difficulties[global.diff_selected]));

    // Should have something like. Permanent Container that run this for every frame (Set something permanent)
    System::ECS::ResourcePool<1000, Game::World::SaveState> &save_state =  manager->get_rm()->query<Game::World::SaveState>();
    for (auto [id, save]: save_state)
        rm.add_resource(id, std::move(save));

    return rm;
}