#pragma once
#include "game.h"

namespace Scene
{
    Game::Battle::BulletRegistry init_bullet_graphic();

    Game::Battle::PatternContainer create_pattern_container();

    Game::Battle::BulletLoader create_bullet_data();

    Game::Battle::BulletLoader create_bullet_data2();

    struct DemoBulletHell
    {
        static DemoBulletHell instance();

        constexpr static auto name = "DemoBulletHell";
        constexpr static size_t MaxResource = 12000;
        // declare scene parameters
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletRegistry,
            Game::Battle::BulletLoader,
            Game::Battle::PatternContainer,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData,
            Game::Battle::TransitionData,
            Game::BulletHell::Bullet,
            Game::BulletHell::BulletClearer,
            Game::BulletHell::Input,
            Game::Rhythm::KeyInput,
            Game::BulletHell::Player,
            Game::BulletHell::Pattern,
            Game::BulletHell::Bounce,
            Game::BulletHell::Homing,
            Game::BulletHell::Particle,
            Game::BulletHell::Delay,
            Game::BulletHell::Booming,
            Game::BulletHell::Laser,
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
            Game::Render::Flicker,
            Game::Render::Camera2D,
            Game::Rhythm::NoteType,
            Game::Audio::SoundRegistry,
            Game::Test::FpsCounter,
            Game::Test::BulletCounter
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::BulletHell::load_bullets<Syscall>,
            Game::Battle::input_system<Syscall>,
            Game::BulletHell::input_to_velocity<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::movement_system<Syscall>,
            Game::BulletHell::acceleration_system<Syscall>,
            Game::BulletHell::rotation_system<Syscall>,
            Game::BulletHell::bullet_collision<Syscall>,
            Game::BulletHell::bullet_clearer_system<Syscall>,
            Game::BulletHell::player_system<Syscall>,
            Game::BulletHell::delay_system<Syscall>,
            Game::BulletHell::bullet_system<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::boomer_system<Syscall>,
            Game::BulletHell::laser_system<Syscall>,
            Game::BulletHell::bounce_pattern_system<Syscall>,
            Game::BulletHell::homing_pattern_system<Syscall>,
            // Game::BulletHell::logging_system<Syscall>,
            Game::Battle::update_global_clock<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Test::bullet_counter<Syscall>,
            Game::Test::fps_counter<Syscall>,
            Game::Render::flickering_system<Syscall>
            >;

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}