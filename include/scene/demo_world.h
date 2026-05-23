#pragma once
#include "game.h"
#include "utils/scene_utils.h"

namespace Scene
{
    struct DemoWorld
    {
        static DemoWorld instance();

        constexpr static auto name = "DemoWorld";
        constexpr static size_t MaxResource = 1000;
        // declare scene parameters
        using ComponentTuple = std::tuple<
            Game::Input,
            Game::Overview::DialogueRegistry,
            Game::Overview::EventRegister,
            Game::Overview::SceneRegistry,
            Game::Overview::Block,
            Game::Overview::DialogueBox,
            Game::Overview::EventState,
            Game::Overview::GlobalState,
            Game::Overview::SaveState,
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
            Game::Physics::Rotation,
            Game::Physics::Velocity,
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
            Game::input_system<Syscall>,
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

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}