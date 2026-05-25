#pragma once

#include "game.h"

namespace Scene
{
    struct Level1
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

        static Level1 instance();

        constexpr static auto name = "Level1";
        constexpr static size_t MaxResource = 10000;

        // declare scene parameters
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletRegistry,
            Game::Battle::BulletLoader,
            Game::Battle::Border,
            Game::Battle::PatternContainer,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData,
            Game::Battle::TransitionData,
            Game::Battle::UIComponent,
            Game::BulletHell::Bullet,
            Game::BulletHell::BulletClearer,
            Game::BulletHell::Input,
            Game::BulletHell::Player,
            Game::BulletHell::PlayerHitbox,
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
            Game::Render::AnimationDataRegistry,
            Game::Render::Animation_Controller,
            Game::Render::Animator,
            Game::Rhythm::NoteType,
            Game::Rhythm::Lane,
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::JudgeText,
            Game::Rhythm::NoteField,
            Game::Rhythm::KeyInput,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::HoldConnect,
            Game::Rhythm::JudgementLine,
            Game::Rhythm::NoteEffect,
            Game::Audio::SoundRegistry,
            Game::Battle::TransitionText
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            return_to_menu<Syscall>,
            Game::Battle::input_system<Syscall>,
            Game::Battle::phase_change<Syscall>,
            Game::Battle::update_global_clock<Syscall>,
            Game::Battle::check_player_state<Syscall>,
            Game::Battle::phase_border_change<Syscall>,
            Game::Battle::phase_player_change<Syscall>,
            Game::Battle::phase_judgement_change<Syscall>,
            Game::BulletHell::load_bullets<Syscall>,
            Game::BulletHell::input_to_velocity<Syscall>,
            Game::BulletHell::movement_system<Syscall>,
            Game::BulletHell::acceleration_system<Syscall>,
            Game::BulletHell::rotation_system<Syscall>,
            Game::BulletHell::bullet_collision<Syscall>,
            Game::BulletHell::bullet_clearer_system<Syscall>,
            Game::BulletHell::player_system<Syscall>,
            Game::BulletHell::player_anim_system<Syscall>,
            Game::BulletHell::delay_system<Syscall>,
            Game::BulletHell::bullet_system<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::boomer_system<Syscall>,
            Game::BulletHell::laser_system<Syscall>,
            Game::BulletHell::pattern_system<Syscall>,
            Game::BulletHell::bounce_pattern_system<Syscall>,
            Game::BulletHell::homing_pattern_system<Syscall>,
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::handle_rain_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_judge_text<Syscall>,
            Game::Rhythm::update_notes<Syscall>,
            Game::Rhythm::handle_note_effect<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::flickering_system<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Render::anim_transition_system<Syscall>,
            Game::Render::animation_system<Syscall>,
            Game::Battle::handle_sprite_ui<Syscall>,
            Game::Battle::handle_text_ui<Syscall>
            >;

        static Game::Battle::BulletLoader create_bullet_test();

        static void load_chart(
            std::shared_ptr<TaskManager> &tm,
            Game::Battle::ChartData &chart,
            Game::Rhythm::NoteField &field);

        // static Game::Battle::Difficulty set_difficulty(int level);

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}