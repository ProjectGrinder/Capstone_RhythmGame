#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{
    template <typename T>
    void create_demo_chart([[maybe_unused]] T &syscall, Game::Battle::ChartData &chart)
    {
        // Configure ChartData
        for (int lane = 0; lane < 4; ++lane)
        {
            chart.lanes[lane].lane_number = lane;
            chart.lanes[lane].notes.clear();
            chart.lanes[lane].current_note = 0;
        }

        // Pattern 1: Simple taps
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 1000, 0));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(false, 2000, 0));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(false, 3000, 0));
        chart.lanes[3].notes.push_back(Game::Battle::NoteData(false, 4000, 0));

        // Pattern 2: Holds
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(true, 5000, 6000));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(true, 7000, 8000));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(true, 9000, 10500));
        chart.lanes[3].notes.push_back(Game::Battle::NoteData(true, 10500, 12000));

        // Pattern 3: Simultaneous notes
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 13000, 0));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(false, 13000, 0));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(false, 15000, 0));
        chart.lanes[3].notes.push_back(Game::Battle::NoteData(false, 15000, 0));
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(true, 17000, 19000));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(true, 17000, 19000));

        // Pattern 4: Repeated taps
        for (int i = 0; i < 3; ++i)
        {
            chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 20000 + i * 500, 0));
        }
        for (int j = 0; j < 3; ++j)
        {
            chart.lanes[3].notes.push_back(Game::Battle::NoteData(false, 24000 + i * 500, 0));
        }

        // Pattern 5: Mixed notes
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 28000, 0));
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(true, 29000, 30000));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(false, 30000, 0));
        chart.lanes[1].notes.push_back(Game::Battle::NoteData(true, 31000, 32000));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(true, 32000, 33000));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(false, 33500, 0));   
        chart.lanes[3].notes.push_back(Game::Battle::NoteData(true, 34000, 35000));
        chart.lanes[3].notes.push_back(Game::Battle::NoteData(false, 35500, 0));
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 37000, 0));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(true, 37000, 38000));
        chart.lanes[0].notes.push_back(Game::Battle::NoteData(false, 37000, 0));
        chart.lanes[2].notes.push_back(Game::Battle::NoteData(true, 37000, 38000));
    }

    template <typename T>
    void setup_demo_rhythm_scene([[maybe_unused]] T &syscall)
    {
        // Create and configure BattleState
        auto &battle_state = syscall.create_component<Game::Battle::BattleState>();
        battle_state = Game::Battle::BattleState(100, 100, Game::Battle::NO_INSTRUMENT);

        // Create and configure RhythmState
        auto &rhythm_state = syscall.create_component<Game::Battle::RhythmState>();
        rhythm_state = Game::Battle::RhythmState(1, 1, 32, 1.0f);

        // Create and configure ChartData
        auto &chart_data = syscall.create_component<Game::Battle::ChartData>();
        create_demo_chart(syscall, chart_data);

        // Create KeyInput component
        syscall.create_component<Game::Rhythm::KeyInput>();

        // Create Lane and NoteSpeed components
        syscall.create_component<Game::Rhythm::Lane>(0);
        syscall.create_component<Game::Rhythm::Lane>(1);
        syscall.create_component<Game::Rhythm::Lane>(2);
        syscall.create_component<Game::Rhythm::Lane>(3);
        syscall.create_component<Game::Rhythm::NoteSpeed>(1.0f);
    }

    struct DemoRhythmScene
    {
        static DemoRhythmScene instance();

        constexpr static auto name = "DemoRhythmScene";
        // declare scene parameters
        constexpr static size_t MaxResource = 500;
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Rhythm::KeyInput,
            Game::Rhythm::Lane,
            Game::Rhythm::NoteSpeed,
            Game::Rhythm::Timing,
            Game::Rhythm::TimingEnd,
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::InputSystem<Syscall>,
            Game::Rhythm::HandleRhythm<Syscall>,
            Game::Rhythm::HandleMissNote<Syscall>,
            Game::Rhythm::HandleBPM<Syscall>
            >;
    };
}