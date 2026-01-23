#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{
    template <typename T>
    void CreateDemoChart([[maybe_unused]] T &syscall, Game::Battle::ChartData &chart)
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
}