#pragma once
#include "game.h"

namespace Scene
{
    inline Game::Overview::DialogueRegistry init_dialogue_registry()
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

    inline Game::Overview::EventRegister init_event_registry()
    {
        using namespace Game::Overview;
        EventRegister event_sequences = {
            {LockInputEvent(0b100), DialogueEvent(0), DialogueEvent(1), DialogueEvent(2), ChangeNextEvent(1), UnlockInputEvent()},
            {LockInputEvent(0b100), DialogueEvent(3), UnlockInputEvent(), ChangeNextEvent(2)},
            {LockInputEvent(0b100), DialogueEvent(4), UnlockInputEvent()}
        };
        return { EventRegister(event_sequences) };
    }

    inline Game::Overview::SceneRegistry init_scene_registry()
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

    struct DemoOverworld
    {
        static DemoOverworld instance();

        constexpr static auto name = "DemoOverworld";
        constexpr static size_t MaxResource = 1000;
        // declare scene parameters
        using ComponentTuple = std::tuple<
            Game::Overview::DialogueRegistry,
            Game::Overview::EventRegister,
            Game::Overview::SceneRegistry,
            Game::Overview::Block,
            Game::Overview::DialogueBox,
            Game::Overview::EventState,
            Game::Overview::GlobalState,
            Game::Overview::SaveState,
            Game::Overview::Input,
            Game::Overview::Interactable,
            Game::Overview::LevelNode,
            Game::Overview::Player,
            Game::Overview::PlayerStat,
            Game::Overview::DialogueEvent,
            Game::Overview::CutSceneEvent,
            Game::Overview::LockInputEvent,
            Game::Overview::UnlockInputEvent,
            Game::Overview::PanCameraEvent,
            Game::Overview::SceneChangeEvent,
            Game::Overview::LevelNodeEvent,
            Game::Overview::ChangeNextEvent,
            Game::Physics::Acceleration,
            Game::Physics::CircularCollider,
            Game::Physics::RectangularCollider,
            Game::Physics::AngularVelocity,
            Game::Physics::Position,
            Game::Physics::Rotation,
            Game::Physics::Velocity,
            Game::Physics::Scale,
            Game::Render::Transform,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D,
            Game::Battle::BattleState
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Overview::load_scene_objects<Syscall>,
            Game::Overview::input_system<Syscall>,
            Game::Overview::player_move<Syscall>,
            Game::BulletHell::acceleration_system<Syscall>,
            Game::BulletHell::rotation_system<Syscall>,
            Game::BulletHell::movement_system<Syscall>,
            Game::Overview::event_system<Syscall>,
            Game::Overview::detect_overlap_block<Syscall>,
            Game::Overview::detect_overlap_interactable<Syscall>,
            Game::Overview::dialogue_system<Syscall>,
            Game::Overview::dialogue_box_system<Syscall>,
            Game::Overview::lock_event_system<Syscall>,
            Game::Overview::logging_system<Syscall>
            >;

        static std::shared_ptr<TaskManager> init()
        {
            auto tm = std::make_shared<TaskManager>();
            // Create and configure BattleState
            tm->create_entity<Game::Overview::DialogueRegistry,
            Game::Overview::EventRegister,
            Game::Overview::SceneRegistry,
            Game::Overview::GlobalState,
            Game::Overview::Input,
            Game::Overview::SaveState,
            Game::Overview::PlayerStat,
            Game::Battle::BattleState>
            (
                init_dialogue_registry(),
                init_event_registry(),
                init_scene_registry(),
                Game::Overview::GlobalState(),
                Game::Overview::Input(),
                Game::Overview::SaveState(),
                Game::Overview::PlayerStat(),
                Game::Battle::BattleState()
                );

            tm->create_entity<Game::Overview::Player,
            Position, Rotation,
            Game::Physics::Scale,
            Velocity,
            Acceleration,
            AngularVelocity, Game::Physics::CircularCollider>(
                {}, {}, {}, {}, {}, {}, {}, {}
            );
            tm->run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> exit();
    };
}