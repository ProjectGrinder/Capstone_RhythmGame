#pragma once

#include "game.h"

namespace Scene
{
    Game::Render::Sprite assign_sprite(int type);

    struct DemoGame
    {
        template<typename T>
        static void return_to_menu(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Game::Battle::BattleState> &battle_query)
        {
            if (battle_query.begin() == battle_query.end())
                return;

            if (battle_query.front().get<Game::Battle::BattleState>().player_state == Game::Battle::PlayerState::PLAY)
                return;

            constexpr auto ESC = 0x1B;

            if (get_key_state(ESC))
            {
                LOG_INFO("Escape pressed");
                Scene::queue_change_scene<DemoMenu>();
            }
        }

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
            Game::Render::Triangle,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D,
            Game::Render::Flicker,
            Game::Rhythm::NoteType,
            Game::Rhythm::Lane,
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::JudgeText,
            Game::Rhythm::Combo,
            Game::Rhythm::NoteField,
            Game::Rhythm::KeyInput,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::HoldConnect,
            Game::Audio::SoundRegistry,
            Game::Test::LifeText,
            Game::Test::GrazeText,
            Game::Battle::HpBarMax,
            Game::Battle::HpBar,
            Game::Battle::Score,
            Game::Battle::UIDisplay
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            return_to_menu<Syscall>,
            Game::Battle::input_system<Syscall>,
            Game::Battle::phase_change<Syscall>,
            Game::Battle::update_global_clock<Syscall>,
            Game::Battle::check_player_state<Syscall>,
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
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_judge_text<Syscall>,
            Game::Rhythm::update_combo<Syscall>,
            Game::Rhythm::update_notes<Syscall>,
            Game::Render::flickering_system<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Battle::update_score<Syscall>
            >;

        static Game::Battle::BulletLoader create_bullet_test();
        static Game::Battle::ChartData create_note_test();

        static void load_chart(
            std::shared_ptr<TaskManager> &tm,
            Game::Battle::ChartData &chart,
            Game::Rhythm::NoteField &field);

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}