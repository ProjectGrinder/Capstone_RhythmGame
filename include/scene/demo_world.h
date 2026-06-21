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
            Game::BulletHell::Particle,
            Game::World::LevelRegistry,
            Game::World::DialogueRegistry,
            Game::World::EventRegister,
            Game::World::SceneRegistry,
            Game::World::Block,
            Game::World::CameraAttractor,
            Game::World::DialogueBox,
            Game::World::EventState,
            Game::World::GlobalState,
            Game::World::SaveState,
            Game::World::Interactable,
            Game::World::Player,
            Game::World::PlayerStat,
            Game::World::DialogueEvent,
            Game::World::CutSceneEvent,
            Game::World::LockInputEvent,
            Game::World::UnlockInputEvent,
            Game::World::PanCameraEvent,
            Game::World::SceneChangeEvent,
            Game::World::LevelNodeEvent,
            Game::World::ChangeNextEvent,
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
            Game::Render::Resize,
            Game::Battle::BattleState,
            Game::Battle::LevelData
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::update_global_clock<Syscall>,
            Game::World::load_scene_objects<Syscall>,
            Game::input_system<Syscall>,
            Game::World::player_move<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::acceleration_system<Syscall>,
            Game::BulletHell::rotation_system<Syscall>,
            Game::BulletHell::movement_system<Syscall>,
            Game::World::camera_follow<Syscall>,
            Game::World::detect_overlap_block<Syscall>,
            Game::World::detect_overlap_interactable<Syscall>,
            Game::World::dialogue_system<Syscall>,
            Game::World::dialogue_box_system<Syscall>,
            Game::World::lock_event_system<Syscall>,
            Game::World::level_node_system<Syscall>,
            Game::World::change_event_system<Syscall>,
            Game::World::event_system<Syscall>,
            // Game::World::logging_system<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Render::resize_system<Syscall>
            // Game::Test::draw_collider<Syscall>
            >;

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}