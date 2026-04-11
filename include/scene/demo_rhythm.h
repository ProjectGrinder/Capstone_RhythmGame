#pragma once
#include "../../src/windows/windows_types.h"
#include "game.h"
#include "system.h"

extern "C" Window get_window_size();

namespace Scene
{
    inline Game::Battle::ChartData create_demo_chart()
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
        // Pattern 5: Mixed notes
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

    inline Game::Battle::BattleState create_battle_state()
    {
        Game::Battle::BattleState state(100, 100, Game::Battle::Difficulty());
        state.current_phase = Game::Battle::RHYTHM;
        return (state);
    };
    inline Game::Battle::RhythmState create_rhythm_state()
    {
        Game::Battle::RhythmState state(1, 100, 60, 2.0f, 2.0f);
        state.accept_loss.normal = 5;
        state.accept_loss.accent = 5;
        state.accept_loss.rain = 2;
        state.accept_loss.hold = 5;
        state.accept_loss.hold_end = 2;
        return (state);
    };

    inline Game::Rhythm::NoteField create_field()
    {
        // position based on window size
        const float half_height = static_cast<float>(get_window_size().height) / 2;
        const float half_width = static_cast<float>(get_window_size().width) / 2;
        const float spawn_level = half_height;
        const float judge_level = half_height * -4 / 5;
        const float lane1_spawn = half_width * -3 / 10;
        const float lane2_spawn = half_width * -1 / 10;
        const float lane3_spawn = half_width * 1 / 10;
        const float lane4_spawn = half_width * 3 / 10;
        return Game::Rhythm::NoteField(
            spawn_level,
            judge_level,
            lane1_spawn,
            lane2_spawn,
            lane3_spawn,
            lane4_spawn);
    }

    inline Game::Battle::BpmInfo create_bpm_info()
    {
        Game::Battle::BpmInfo bpm;
        Game::Battle::BpmInfo::InfoPair info{};
        info.bpm = 134.00f;
        info.timing = 0;
        bpm.bpm_list.emplace_back(info);
        return (bpm);
    }

    inline Game::Battle::PhaseInfo create_phase_info()
    {
        Game::Battle::PhaseInfo phase;
        Game::Battle::PhaseInfo::InfoPair info{};
        info.phase = Game::Battle::RHYTHM;
        info.timing = 0;
        phase.phase_list.emplace_back(info);
        return (phase);
    }

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
            Game::Rhythm::Lane,
            Game::Rhythm::Timing,
            Game::Rhythm::HoldStart,
            Game::Rhythm::NoteType,
            Game::Rhythm::NoteStatus,
            Game::Rhythm::NoteField,
            Game::Render::Camera2D,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Transform
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::input_system<Syscall>,
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Rhythm::handle_tap_note<Syscall>,
            Game::Rhythm::set_holding_time<Syscall>,
            Game::Rhythm::handle_holding<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::update_notes<Syscall>,
            Game::Render::set_camera<Syscall>,
            Game::Rhythm::draw_notes<Syscall>,
            Game::Render::draw_text<Syscall>,
            Game::Battle::update_global_clock<Syscall>
            >;

        static void load_chart(
            std::shared_ptr<TaskManager> &tm,
            Game::Battle::ChartData &chart,
            Game::Rhythm::NoteField &field);

        static std::shared_ptr<TaskManager> init()
        {
            auto tm = std::make_shared<TaskManager>();
            tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

            tm->create_entity<Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Rhythm::KeyInput,
            Game::BulletHell::Input>
            (
                create_battle_state(),
                create_rhythm_state(),
                create_demo_chart(),
                Game::Rhythm::KeyInput(),
                Game::BulletHell::Input());

            tm->create_entity<Game::Battle::LevelData>(Game::Battle::LevelData(
                "A World Without You",
                "Nakuya",
                "Digital Jpop",
                134.00f,
                create_bpm_info(),
                std::vector<Game::Battle::Difficulty>()));

            // Create Lane
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

            tm->create_entity<Game::Rhythm::NoteField>(create_field());

            auto chart = create_demo_chart();
            auto field = create_field();

            load_chart(tm, chart, field);

            tm->run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> exit();
    };
}