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

    load_sprite("img/Square.dds", "Platform", 64, 64);
    load_sprite("img/test.dds", "Npc", 420, 420);

    load_sprite("img/Square.dds", "Square", 64, 64);
    load_sprite("img/Square64px.dds", "Square64px", 64, 64);
    load_sprite("img/ring16px.dds", "ring16px", 72, 72);

    load_font("fonts/Klub04TT-NoBG.dds", "Klub04TT-NoBG", "fonts/Klub04TT-Normal.txt");
}

Game::World::DialogueRegistry init_dialogue_registry()
{
    using namespace Game::World;
    std::vector<std::string> text_register = {
        "This is a place holder text to test the system",
        "This is not a place for you to be gooning",
        "just...",
        "become detroit.... destroy human",
        "Fun time at poppy playtime co."
    };
    return {DialogueRegistry(text_register)};
}

Game::World::EventRegister init_event_registry()
{
    using namespace Game::World;
    EventRegister event_sequences = {
        {LockInputEvent(0b100), DialogueEvent(0), DialogueEvent(1), DialogueEvent(2), UnlockInputEvent(), ChangeNextEvent(1)},
        {LockInputEvent(0b100), DialogueEvent(3), UnlockInputEvent()},
        {LockInputEvent(0b100), DialogueEvent(4), LevelNodeEvent(0), UnlockInputEvent()}
    };
    return { EventRegister(event_sequences) };
}

Game::World::SceneRegistry init_scene_registry()
{
    using namespace Game::World;
    const SceneRegistry scene_registry = {
        {
            SceneObject(0,64*-1,100,1,2, {Platform}, {}),
            SceneObject(-6400,64*6,1,12, 2, {Platform}, {}),
            SceneObject(3600,64*0,50,1, 2, {Platform}, {}),
            SceneObject(7200,64*1,25,1, 2, {Platform}, {}),
            SceneObject(720,64*2,2,0.5, 2, {Platform}, {}),
            SceneObject(1080,64*3,2,0.5, 2, {Platform}, {}),
            SceneObject(6400,64*6,1,12, 2, {Platform}, {}),
            SceneObject(1080,64*4.5,0.2f,0.2f, 1, 0, {Npc}, {}),
            SceneObject(64*0,64*1,0.2f,0.2f, 1, 2, {Npc}, {}),
        }
    };
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
    tm->create_entity(Game::Render::Camera2D{.offset = {5,5}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});
    init_graphics(tm);

    // Create and configure BattleState
    tm->create_entity<Game::World::DialogueRegistry,
    Game::World::EventRegister,
    Game::World::SceneRegistry,
    Game::World::GlobalState,
    Game::World::PlayerStat>
    (
        init_dialogue_registry(),
        init_event_registry(),
        init_scene_registry(),
        Game::World::GlobalState(),
        Game::World::PlayerStat()
        );

    // SaveState
    tm->create_entity<Game::World::SaveState>(Game::World::SaveState());
    // InputManager
    tm->create_entity<Game::Input>(Game::Input());
    // LevelRegistry
    // TODO : Move this to init at the start of the game
    tm->create_entity<Game::World::LevelRegistry>({{
        Game::Battle::LevelData("A World Without You", "Nakuya", "Digital Jpop", 134.00f,
        Game::Battle::BpmInfo({{0,134}}), {{Game::Battle::LIGHT,1},{Game::Battle::SPARK,3},{Game::Battle::BLAZE,5}}, 142000)
    }});

    tm->create_entity<Game::World::Player, Rotation,
    Game::Render::Transform,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material>(
        {},{}, {0,100}, {}, Acceleration(0,0,1000,1000,-1000,-1000), {}, Game::Physics::CircularCollider(20.f,32.f),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 1,
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
    free_assets(get_assets_id("Npc"));

    // ResourceManager rm;
    //
    // // Should have something like. Permanent Container that run this for every frame (Set something permanent)
    // System::ECS::ResourceManager<
    //         1000,
    //         Game::Input,
    //         Game::World::DialogueRegistry,
    //         Game::World::EventRegister,
    //         Game::World::SceneRegistry,
    //         Game::World::Block,
    //         Game::World::CameraAttractor,
    //         Game::World::DialogueBox,
    //         Game::World::EventState,
    //         Game::World::GlobalState,
    //         Game::World::SaveState,
    //         Game::World::Interactable,
    //         Game::World::Player,
    //         Game::World::PlayerStat,
    //         Game::World::DialogueEvent,
    //         Game::World::CutSceneEvent,
    //         Game::World::LockInputEvent,
    //         Game::World::UnlockInputEvent,
    //         Game::World::PanCameraEvent,
    //         Game::World::SceneChangeEvent,
    //         Game::World::LevelNodeEvent,
    //         Game::World::ChangeNextEvent,
    //         Acceleration,
    //         Game::Physics::CircularCollider,
    //         Game::Physics::RectangularCollider,
    //         AngularVelocity,
    //         Rotation,
    //         Velocity,
    //         Game::Render::Transform,
    //         Game::Render::Sprite,
    //         Game::Render::Material,
    //         Game::Render::Text,
    //         Game::Render::Camera2D,
    //         Game::Battle::BattleState> // I have to add this just because I have to send this INFO???
    //         *current = manager->get_rm();
    // System::ECS::ResourcePool<1000, Game::World::SaveState> &pool = current->query<Game::World::SaveState>();


    return ResourceManager();
}