#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{
    inline Game::Battle::ChartData create_demo_chart()
    {
        Game::Battle::ChartData chart;
        // Configure ChartData
        for (int lane = 0; lane < 4; ++lane)
        {
            chart.lanes[lane].lane_number = lane;
            chart.lanes[lane].notes.clear();
            chart.lanes[lane].current_note = 0;
        }

        // Pattern 1: Simple taps
        chart.lanes[0].notes.emplace_back(false, 1000, 0);
        chart.lanes[1].notes.emplace_back(false, 2000, 0);
        chart.lanes[2].notes.emplace_back(false, 3000, 0);
        chart.lanes[3].notes.emplace_back(false, 4000, 0);

        // Pattern 2: Holds
        chart.lanes[0].notes.emplace_back(true, 5000, 6000);
        chart.lanes[1].notes.emplace_back(true, 7000, 8000);
        chart.lanes[2].notes.emplace_back(true, 9000, 10500);
        chart.lanes[3].notes.emplace_back(true, 10500, 12000);

        // Pattern 3: Simultaneous notes
        chart.lanes[0].notes.emplace_back(false, 13000, 0);
        chart.lanes[1].notes.emplace_back(false, 13000, 0);
        chart.lanes[2].notes.emplace_back(false, 15000, 0);
        chart.lanes[3].notes.emplace_back(false, 15000, 0);
        chart.lanes[0].notes.emplace_back(true, 17000, 19000);
        chart.lanes[1].notes.emplace_back(true, 17000, 19000);

        // Pattern 4: Repeated taps
        for (int i = 0; i < 3; ++i)
        {
            chart.lanes[0].notes.emplace_back(false, 20000 + i * 500, 0);
        }
        for (int j = 0; j < 3; ++j)
        {
            chart.lanes[3].notes.emplace_back(false, 24000 + j * 500, 0);
        }

        // Pattern 5: Mixed notes
        chart.lanes[0].notes.emplace_back(false, 28000, 0);
        chart.lanes[0].notes.emplace_back(true, 29000, 30000);
        chart.lanes[1].notes.emplace_back(false, 30000, 0);
        chart.lanes[1].notes.emplace_back(true, 31000, 32000);
        chart.lanes[2].notes.emplace_back(true, 32000, 33000);
        chart.lanes[2].notes.emplace_back(false, 33500, 0);
        chart.lanes[3].notes.emplace_back(true, 34000, 35000);
        chart.lanes[3].notes.emplace_back(false, 35500, 0);
        chart.lanes[0].notes.emplace_back(false, 37000, 0);
        chart.lanes[2].notes.emplace_back(true, 37000, 38000);
        chart.lanes[0].notes.emplace_back(false, 37000, 0);
        chart.lanes[2].notes.emplace_back(true, 37000, 38000);

        return (chart);
    }

    template <typename T>
    void setup_demo_rhythm_scene([[maybe_unused]] T &syscall)
    {

    }

    template <typename T>
    void time_check([[maybe_unused]] T &syscall, System::ECS::Query<Game::Battle::BattleState> &query)
    {
        if (query.begin() == query.end())
        {
            return;
        }
        auto time = query.front().get<Game::Battle::BattleState>().clock_time;
        if (time % 1000 == 0)
        {
            LOG_INFO("Current Time: %d s", time);
        }
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
            Game::Rhythm::HoldActive
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::InputSystem<Syscall>,
            Game::Rhythm::HandleRhythm<Syscall>,
            Game::Rhythm::HandleMissNote<Syscall>,
            Game::Rhythm::HandleBPM<Syscall>,
            time_check<Syscall>
            >;
        
        static TaskManager Init()
        {
            auto tm = TaskManager{};
            // Create and configure BattleState
            tm.create_entity<Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Rhythm::KeyInput, Game::Rhythm::NoteSpeed>
            (
                Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
                Game::Battle::RhythmState(1, 1, 32, 1.0f),
                create_demo_chart(),
                Game::Rhythm::KeyInput(),
                Game::Rhythm::NoteSpeed(1.0f));


            // Create Lane and NoteSpeed components
            tm.create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
            tm.create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
            tm.create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
            tm.create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));
            tm.run_all();
            return tm;
        }

        static std::vector<ComponentTuple> Exit();
    };
}