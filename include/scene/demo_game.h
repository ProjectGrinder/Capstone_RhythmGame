#pragma once
#include "demo_rhythm.h"
#include "game.h"

namespace Scene
{
    struct DemoGame
    {
        static DemoGame instance();

        constexpr static auto name = "DemoGame";
        constexpr static size_t MaxResource = 1000;

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
            Game::Physics::Position,
            Game::Physics::Rotation,
            Game::Physics::Velocity,
            Game::Physics::Scale,
            Game::Render::Transform,
            Game::Render::Triangle,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D,
            Game::Rhythm::NoteType,
            Game::Rhythm::Lane,
            Game::Rhythm::NoteSpeed,
            Game::Rhythm::Timing,
            Game::Rhythm::TimingEnd,
            Game::Rhythm::HoldActive
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::input_system<Syscall>,
            Game::Battle::phase_change<Syscall>,
            Game::Battle::update_global_clock<Syscall>,
            Game::BulletHell::load_bullets<Syscall>,
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
            Game::BulletHell::logging_system<Syscall>,
            Game::Rhythm::load_notes<Syscall>,
            Game::Rhythm::handle_rhythm<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>
            >;

        static std::shared_ptr<TaskManager> init()
        {
            auto tm = std::make_shared<TaskManager>();

            auto config_battle_state = []
            {
                Game::Battle::BattleState state(100, 100, Game::Battle::Difficulty());
                state.current_phase = Game::Battle::RHYTHM;
                return (state);
            };

            // Create and configure BattleState
            tm->create_entity<Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletRegistry,
            Game::Battle::BulletLoader,
            Game::Battle::PatternContainer,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Rhythm::KeyInput, Game::BulletHell::Input>
            (
                config_battle_state(),
                Game::Battle::BulletHellState(),
                init_bullet_graphic(),
                create_bullet_data(),
                create_pattern_container(),
                Game::Battle::RhythmState(1, 1, 60, 1.0f),
                create_demo_chart(),
                Game::Rhythm::KeyInput(),
                Game::BulletHell::Input()
                );

            tm->create_entity<Game::BulletHell::Player,
            Position, Rotation,
            Game::Physics::Scale,
            Velocity,
            Acceleration,
            AngularVelocity, Game::Physics::CircularCollider>(
                {}, Position(50,50), {}, {}, {}, {}, {}, {}
            );

            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

            tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(35000,5000,Game::Battle::BULLET_HELL));
            tm->run_all();
            return (tm);
        }

        static std::shared_ptr<TaskManager> init([[maybe_unused]] std::vector<ComponentTuple> &data)
        {
            return (init());
        }

        static std::vector<ComponentTuple> exit(std::shared_ptr<TaskManager> &manager);
    };
}