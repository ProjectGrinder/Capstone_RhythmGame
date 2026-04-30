#pragma once
#include "demo_rhythm.h"
#include "game.h"

namespace Scene
{
    struct DemoGame
    {
        static DemoGame instance();

        static Game::Battle::ChartData create_demo_chart()
        {
            Game::Battle::ChartData chart;

            for (int lane = 0; lane < 4; ++lane)
            {
                chart.lanes[lane].lane_number = lane;
                chart.lanes[lane].notes.clear();
                chart.lanes[lane].current_note = 0;
            }

            // Pattern 1: Simple taps
            chart.lanes[0].notes.emplace_back(false, 5000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[1].notes.emplace_back(false, 6000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[2].notes.emplace_back(false, 7000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[3].notes.emplace_back(false, 8000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[0].notes.emplace_back(false, 9000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[1].notes.emplace_back(false, 9000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[2].notes.emplace_back(false, 11000, 0, Game::Battle::RhythmType::NORMAL);
            chart.lanes[3].notes.emplace_back(false, 11000, 0, Game::Battle::RhythmType::NORMAL);

            // Pattern 2: Accent notes
            chart.lanes[0].notes.emplace_back(false, 13000, 0, Game::Battle::RhythmType::ACCENT);
            chart.lanes[1].notes.emplace_back(false, 14000, 0, Game::Battle::RhythmType::ACCENT);
            chart.lanes[2].notes.emplace_back(false, 15000, 0, Game::Battle::RhythmType::ACCENT);
            chart.lanes[3].notes.emplace_back(false, 16000, 0, Game::Battle::RhythmType::ACCENT);
            chart.lanes[1].notes.emplace_back(false, 17000, 0, Game::Battle::RhythmType::ACCENT);
            chart.lanes[2].notes.emplace_back(false, 17000, 0, Game::Battle::RhythmType::ACCENT);

            // Pattern 3: Rain notes (only taps)
            for (int m = 0; m < 10; ++m)
            {
                chart.lanes[0].notes.emplace_back(false, 19000 + m * 100, 0, Game::Battle::RhythmType::RAIN);
            }
            for (int n = 0; n < 20; ++n)
            {
                chart.lanes[3].notes.emplace_back(false, 21000 + n * 50, 0, Game::Battle::RhythmType::RAIN);
            }

            // Pattern 4: Holds
            chart.lanes[0].notes.emplace_back(true, 24000, 25000, Game::Battle::RhythmType::NORMAL);
            chart.lanes[1].notes.emplace_back(true, 26000, 27000, Game::Battle::RhythmType::NORMAL);
            chart.lanes[2].notes.emplace_back(true, 28000, 30000, Game::Battle::RhythmType::NORMAL);
            chart.lanes[3].notes.emplace_back(true, 29000, 30000, Game::Battle::RhythmType::NORMAL);
            chart.lanes[0].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
            chart.lanes[1].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
            chart.lanes[2].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
            chart.lanes[3].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);

            /*
            Pattern 5: Mixed notes
            chart.lanes[0].notes.emplace_back(false, 24000, 0);
            chart.lanes[0].notes.emplace_back(true, 25000, 26000);
            chart.lanes[1].notes.emplace_back(false, 26000, 0);
            chart.lanes[1].notes.emplace_back(true, 27000, 28000);
            chart.lanes[2].notes.emplace_back(true, 28000, 29000);
            chart.lanes[2].notes.emplace_back(false, 29500, 0);
            chart.lanes[3].notes.emplace_back(true, 30000, 31000);
            chart.lanes[3].notes.emplace_back(false, 31500, 0);
            chart.lanes[0].notes.emplace_back(false, 33000, 0);
            chart.lanes[2].notes.emplace_back(true, 33000, 34000);
            chart.lanes[0].notes.emplace_back(false, 35000, 0);
            chart.lanes[2].notes.emplace_back(true, 35000, 36000);
            */

            return (chart);
        }
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
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::JudgeText,
            Game::Rhythm::Combo,
            Game::Rhythm::NoteField,
            Game::Rhythm::KeyInput,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::HoldConnect
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
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_judge_text<Syscall>,
            Game::Rhythm::update_combo<Syscall>,
            Game::Rhythm::update_notes<Syscall>
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
                Game::Battle::RhythmState(1, 1, 60, 1.0f, 1.0f),
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

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}