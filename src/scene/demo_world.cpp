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
    load_sprite("img/tripleT.dds", "npc", 420, 420);
    load_sprite("img/bullethell/BH_Player_Sprite.dds", "BH_Player_Sprite", 800, 1500);

    load_sprite("img/Square.dds", "Square", 64, 64);
    load_sprite("img/Square64px.dds", "Square64px", 64, 64);
    load_sprite("img/ring16px.dds", "ring16px", 72, 72);

    load_sprite("img/return.dds", "return", 1280, 720);

    load_sprite("img/level1_bg.dds", "level1_bg", 3840, 2160);
}

Game::Overview::DialogueRegistry init_dialogue_registry()
{
    using namespace Game::Overview;
    std::vector<std::string> text_register = {
        "This is a place holder text to test the system",
        "This is not a place for you to be gooning",
        "just...",
        "become detroit.... destroy human",
        "Fun time at poppy playtime co."
    };
    return {DialogueRegistry(text_register)};
}

Game::Overview::EventRegister init_event_registry()
{
    using namespace Game::Overview;
    EventRegister event_sequences = {
        {LockInputEvent(0b100), DialogueEvent(0), DialogueEvent(1), DialogueEvent(2), ChangeNextEvent(1), UnlockInputEvent()},
        {LockInputEvent(0b100), DialogueEvent(3), UnlockInputEvent(), ChangeNextEvent(2)},
        {LockInputEvent(0b100), DialogueEvent(4), UnlockInputEvent()}
    };
    return { EventRegister(event_sequences) };
}

Game::Overview::SceneRegistry init_scene_registry()
{
    using namespace Game::Overview;
    const SceneRegistry scene_registry = {
        {
            SceneObject(0,-10, 2, {}, {}),
            SceneObject(-1,-10, 2, {}, {}),
            SceneObject(1,-10, 2, {}, {}),
            SceneObject(-2,-10, 2, {}, {}),
            SceneObject(2,-10, 2, {}, {}),
            SceneObject(-3,-10, 2, {}, {}),
            SceneObject(3,-10, 2, {}, {}),
            SceneObject(-4,-10, 2, {}, {}),
            SceneObject(4,-10, 2, {}, {}),
            SceneObject(5,-10, 2, {}, {}),
            SceneObject(6,-7.5, 2, {}, {}),
            SceneObject(7,-7.5, 2, {}, {}),
            SceneObject(8,-7.5, 2, {}, {}),
            SceneObject(9,-7.5, 2, {}, {}),
            SceneObject(10,-7.5, 2, {}, {}),
            SceneObject(-5,-10, 2, {}, {}),
            SceneObject(-5,-9, 2, {}, {}),
            SceneObject(-5,-8, 2, {}, {}),
            SceneObject(-5,-7, 2, {}, {}),
            SceneObject(-5,-6, 2, {}, {}),
            SceneObject(-5,-5, 2, {}, {}),
            SceneObject(-5,-4, 2, {}, {}),
            SceneObject(10,-6.5, 2, {}, {}),
            SceneObject(10,-5.5, 2, {}, {}),
            SceneObject(10,-4.5, 2, {}, {}),
            SceneObject(10,-3.5, 2, {}, {}),
            SceneObject(10,-2.5, 2, {}, {}),
            SceneObject(10,-1.5, 2, {}, {}),
            SceneObject(8,-7.5, 1, 0, {}, {}),
            SceneObject(0,-8.5, 1, 2, {}, {}),
        }
    };
    return scene_registry;

}


Scene::DemoWorld Scene::DemoWorld::instance()
{
    static DemoWorld instance;
    return (instance);
}

Scene::DemoWorld::ResourceManager Scene::DemoWorld::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoGame Scene.");
    return ResourceManager();
}

std::shared_ptr<Scene::DemoWorld::TaskManager> Scene::DemoWorld::init()
{
    auto tm = std::make_shared<TaskManager>();
    // Create and configure BattleState
    tm->create_entity<Game::Overview::DialogueRegistry,
    Game::Overview::EventRegister,
    Game::Overview::SceneRegistry,
    Game::Overview::GlobalState,
    Game::Overview::SaveState,
    Game::Overview::PlayerStat,
    Game::Battle::BattleState>
    (
        init_dialogue_registry(),
        init_event_registry(),
        init_scene_registry(),
        Game::Overview::GlobalState(),
        Game::Overview::SaveState(),
        Game::Overview::PlayerStat(),
        Game::Battle::BattleState()
        );

    // InputManager
    tm->create_entity<Game::Input>(Game::Input());

    tm->create_entity<Game::Overview::Player,
    Game::Render::Transform,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider>(
        {}, {}, {}, Acceleration(0,0,10,10), {}, {}
    );
    tm->run_all();
    return (tm);
}