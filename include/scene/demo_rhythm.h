#pragma once

#include "game.h"

namespace Scene
{
    struct DemoRhythm
    {
        static DemoRhythm instance();

        constexpr static auto name = "DemoRhythm";
        // declare scene parameters
        constexpr static size_t MaxResource = 2000;
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData, // structure required to satisfy HandleBPM
            Game::BulletHell::Input, // structure required to satisfy InputSystem
            Game::Rhythm::KeyInput,
            Game::Rhythm::JudgeText,
            Game::Rhythm::Combo,
            Game::Rhythm::Lane,
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::NoteType,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::NoteField,
            Game::Rhythm::HoldConnect,
            Game::Render::Camera2D,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Transform,
            Game::Audio::SoundRegistry
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::input_system<Syscall>,
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::handle_rain_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_judge_text<Syscall>,
            Game::Rhythm::update_combo<Syscall>,
            Game::Rhythm::update_notes<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Render::draw_sprite<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Battle::update_global_clock<Syscall>
            >;

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