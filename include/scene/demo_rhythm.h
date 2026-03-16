#pragma once
#include "system.h"
#include "game.h"

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
        chart.lanes[0].notes.emplace_back(false, 1000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[1].notes.emplace_back(false, 2000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(false, 3000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[3].notes.emplace_back(false, 4000, 0, Game::Battle::RhythmType::NORMAL);

        // Pattern 2: Holds
        chart.lanes[0].notes.emplace_back(true, 5000, 6000, Game::Battle::RhythmType::NORMAL);
        chart.lanes[1].notes.emplace_back(true, 7000, 8000, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(true, 9000, 10500, Game::Battle::RhythmType::NORMAL);
        chart.lanes[3].notes.emplace_back(true, 10500, 12000, Game::Battle::RhythmType::NORMAL);

        // Pattern 3: Simultaneous notes
        chart.lanes[0].notes.emplace_back(false, 13000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[1].notes.emplace_back(false, 13000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(false, 15000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[3].notes.emplace_back(false, 15000, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[0].notes.emplace_back(true, 17000, 19000, Game::Battle::RhythmType::NORMAL);
        chart.lanes[1].notes.emplace_back(true, 18000, 19000, Game::Battle::RhythmType::NORMAL);

        // Pattern 4: Repeated taps
        // for (int i = 0; i < 3; ++i)
        // {
        //     chart.lanes[0].notes.emplace_back(false, 20000 + i * 100, 0, Game::Battle::RhythmType::NORMAL);
        // }
        // for (int j = 0; j < 3; ++j)
        // {
        //     chart.lanes[3].notes.emplace_back(false, 21000 + j * 100, 0, Game::Battle::RhythmType::NORMAL);
        // }

        // Pattern 5: Accent notes
        chart.lanes[0].notes.emplace_back(false, 22000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[1].notes.emplace_back(false, 23000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[2].notes.emplace_back(false, 24000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[3].notes.emplace_back(false, 25000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[1].notes.emplace_back(false, 26000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[2].notes.emplace_back(false, 26000, 0, Game::Battle::RhythmType::ACCENT);
        chart.lanes[0].notes.emplace_back(true, 27000, 28000, Game::Battle::RhythmType::ACCENT);
        chart.lanes[3].notes.emplace_back(true, 27000, 28000, Game::Battle::RhythmType::ACCENT);

        // Pattern 6: Rain notes (only taps)
        for (int m = 0; m < 10; ++m)
        {
            chart.lanes[0].notes.emplace_back(false, 28000 + m * 100, 0, Game::Battle::RhythmType::RAIN);
        }
        for (int n = 0; n < 10; ++n)
        {
            chart.lanes[0].notes.emplace_back(false, 30000 + n * 100, 0, Game::Battle::RhythmType::RAIN);
        }

        /*
        // Pattern 7: Mixed notes
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

    struct DemoRhythm
    {
        static DemoRhythm instance();

        constexpr static auto name = "DemoRhythm";
        // declare scene parameters
        constexpr static size_t MaxResource = 500;
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData, // structure required to satisfy HandleBPM
            Game::BulletHell::Input, // structure required to satisfy InputSystem
            Game::Rhythm::KeyInput,
            Game::Rhythm::Lane,
            Game::Rhythm::NoteSpeed,
            Game::Rhythm::Timing,
            Game::Rhythm::TimingEnd,
            Game::Rhythm::HoldActive,
            Game::Rhythm::NoteType
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::input_system<Syscall>,
            Game::Rhythm::load_notes<Syscall>,
            Game::Rhythm::handle_rhythm<Syscall>,
            Game::Rhythm::handle_miss_note<Syscall>,
            Game::Rhythm::handle_bpm<Syscall>,
            Game::Battle::update_global_clock<Syscall>
            >;
        
        static std::shared_ptr<TaskManager> init()
        {
            auto tm = std::make_shared<TaskManager>();
            // Create and configure BattleState
            auto config_battle_state = []
            {
                Game::Battle::BattleState state(100, 100, Game::Battle::Difficulty());
                state.current_phase = Game::Battle::RHYTHM;
                return (state);
            };
            tm->create_entity<Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Rhythm::KeyInput,
            Game::BulletHell::Input,
            Game::Rhythm::NoteSpeed>
            (
                config_battle_state(),
                Game::Battle::RhythmState(1, 1, 20, 1.0f),
                create_demo_chart(),
                Game::Rhythm::KeyInput(),
                Game::BulletHell::Input(),
                Game::Rhythm::NoteSpeed(1.0f));


            // Create Lane and NoteSpeed components
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
            tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));
            tm->run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> exit();
    };
}