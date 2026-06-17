#pragma once

#include "game.h"


namespace Scene
{
    // Game::Rhythm::NoteField create_field();
    Game::Render::Sprite assign_sprite(int type);
    Game::Render::Text write_difficulty(Game::Battle::Difficulty difficulty);

    // Use these methods for the demo level
    // Game::Battle::ChartData create_level1_chart();

    Math::Point lane_to_point(int lane);

    struct DemoRhythm
    {
        static DemoRhythm instance();

        constexpr static auto name = "DemoRhythm";
        // declare scene parameters
        constexpr static size_t MaxResource = 2000;
        using ComponentTuple = std::tuple<
            Game::Input,
            Game::Battle::BattleObject,
            Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData, // structure required to satisfy HandleBPM
            Game::Rhythm::JudgeText,
            Game::Rhythm::Lane,
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::NoteType,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::HoldConnect,
            Game::Render::Camera2D,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Transform,
            Game::Audio::SoundRegistry,
            Game::Rhythm::NoteEffect,
            Game::World::GlobalState
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::input_system<Syscall>,
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::handle_rain_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_judge_text<Syscall>,
            Game::Rhythm::update_notes<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::update_global_clock<Syscall>
            >;

        static void load_chart(
            std::shared_ptr<TaskManager> &tm,
            Game::Battle::ChartData &chart);

        static std::shared_ptr<TaskManager> init();

        static std::shared_ptr<TaskManager> init([[maybe_unused]] ResourceManager &data)
        {
            return (init());
        }
        static ResourceManager exit(std::shared_ptr<TaskManager> &manager);
    };
}