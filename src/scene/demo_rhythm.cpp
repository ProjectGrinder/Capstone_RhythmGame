#include "scene.h"
#include "utils/print_debug.h"
#include "../../src/windows/windows_types.h"

using Timing = Game::Rhythm::Timing;
using HoldStart = Game::Rhythm::HoldStart;
using NoteType = Game::Rhythm::NoteType;
using NoteStatus = Game::Rhythm::NoteStatus;

inline Game::Battle::ChartData create_demo_chart()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    // vivid/stasis moment
    chart.lanes[0].notes.emplace_back(false, 5000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5120, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5240, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 5360, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5480, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5600, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 5720, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5840, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5960, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 6080, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6200, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 6320, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 6440, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 6560, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6680, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 6800, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6920, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 7040, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 7160, 11200, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 7640, 11200, Game::Battle::RhythmType::ACCENT);
    for (int m = 0; m < 16; ++m)
    {
        chart.lanes[0].notes.emplace_back(false, 8000 + m * 200, 0, Game::Battle::RhythmType::RAIN);
        chart.lanes[1].notes.emplace_back(false, 8000 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(false, 8100 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[3].notes.emplace_back(false, 8100 + m * 200, 0, Game::Battle::RhythmType::RAIN);
    }
    chart.lanes[1].notes.emplace_back(false, 11200, 0, Game::Battle::RhythmType::NORMAL);

    return (chart);
}

Game::Battle::ChartData Scene::create_level1_chart()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    // RESPONSE 1
    chart.lanes[0].notes.emplace_back(false, 19701, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 19701, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 20037, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 20373, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 20709, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 21045, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 21045, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 21493, 21828, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 21828, 22164, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 22164, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 22388, 22724, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 22724, 23060, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 23060, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 23284, 23619, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 23619, 23955, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 23955, 24179, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 24179, 24627, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 24627, 25075, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 25075, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 25522, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 25970, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 26082, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 26194, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 26306, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 26418, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 26530, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 26642, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 26754, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 26866, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 27201, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 27537, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 27761, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 27873, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 27985, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 28097, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 28209, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 28321, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 28433, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 28545, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[1].notes.emplace_back(false, 28657, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 29104, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 29552, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 29552, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 29776, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 30000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 30000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 30224, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 30448, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 30448, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 30896, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 31343, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 31343, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 31791, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 32239, 32687, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 32239, 32687, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 33134, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 33246, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 33358, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 33470, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33582, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33694, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33806, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33918, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 34030, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 34366, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 34701, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 34925, 35821, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 35373, 35821, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 35821, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 35821, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 36716, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 36716, 37612, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 37052, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 37388, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[1].notes.emplace_back(false, 37164, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 37612, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 38060, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 38507, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 38507, 39403, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 38843, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 39179, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[1].notes.emplace_back(false, 38955, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 39403, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 39851, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 40075, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 40299, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 40299, 41194, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 40634, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 40970, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 40746, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 41194, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 41642, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 42090, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 42090, 42985, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 42425, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[1].notes.emplace_back(false, 42761, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(false, 42537, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 42985, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 43433, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 43881, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 43881, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 44104, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 44328, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 44328, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 44552, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 44776, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 44776, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 45224, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 45672, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 45672, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 45896, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46119, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 46119, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 46343, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46567, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 46567, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 47015, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 47463, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 47910, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 48358, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 48582, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 48806, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 49030, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 49254, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 49254, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 49478, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 49478, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 49701, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 49701, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 49925, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 49925, 0, Game::Battle::RhythmType::ACCENT);

    // RESPONSE 2
    chart.lanes[0].notes.emplace_back(false, 68060, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 68060, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 68396, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 68731, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 68955, 69403, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 69403, 69851, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 69851, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 70187, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 70522, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 70746, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 70746, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 71194, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 71418, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 71642, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 72090, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 72537, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 72537, 72761, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 72761, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 72985, 73209, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 72985, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 73209, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 73433, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 73433, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 73881, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 74328, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 74328, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 74552, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 74776, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 75000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 75224, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 75224, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 75672, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 76119, 76567, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 76567, 77015, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 77015, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 77351, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 77687, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 77910, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 77910, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 78358, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 78582, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 78806, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 79142, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 79478, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 79701, 80149, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 79701, 80149, Game::Battle::RhythmType::ACCENT);

    // RESPONSE 3
    chart.lanes[0].notes.emplace_back(false, 125373, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 125485, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 125709, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 125821, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 126045, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 126269, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 126381, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 126493, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[0].notes.emplace_back(false, 126604, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 126716, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 126828, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 126940, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[2].notes.emplace_back(false, 127052, 0, Game::Battle::RhythmType::RAIN);
    chart.lanes[3].notes.emplace_back(true, 127164, 127612, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 127612, 128060, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 128060, 128507, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 128507, 128955, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 128955, 129403, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 129403, 129851, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 129851, 130299, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 130299, 130746, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 130746, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 130970, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 131194, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 131194, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 131418, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 131642, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 131642, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 131866, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 132090, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 132090, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 132537, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 132761, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 132761, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 132985, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 133209, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 133209, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 133433, 133881, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 133433, 133881, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 133881, 134328, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 133881, 134328, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 134328, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 134552, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 134776, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 135000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 135224, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 135448, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 135672, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 135896, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 136119, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 136119, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 136343, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 136567, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 136567, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 136791, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 137015, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 137015, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 137239, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 137463, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 137463, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 137687, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 137910, 139701, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 137910, 139701, Game::Battle::RhythmType::ACCENT);

    return (chart);
}

inline Game::Battle::BattleState create_battle_state()
{
    Game::Battle::BattleState state(100, 100, Game::Battle::Difficulty());
    state.current_phase = Game::Battle::RHYTHM;
    return (state);
}

inline Game::Battle::RhythmState create_rhythm_state()
{
    Game::Battle::RhythmState state(1, 10, 60, 6.0f, 6.0f);
    state.accept_loss.normal = 5;
    state.accept_loss.accent = 5;
    state.accept_loss.rain = 2;
    state.accept_loss.hold = 5;
    state.accept_loss.hold_end = 2;
    state.apn = 100.00f / static_cast<float>(state.total_notes);
    return (state);
}

Game::Rhythm::NoteField Scene::create_field()
{
    // constexpr float note_width = 150.0f;
    constexpr float padding = 20.0f;
    // position based on window size
    const float spawn_level = Game::HALF_HEIGHT + padding;
    const float judge_level = Game::JUDGE_LEVEL;
    constexpr float lane1_spawn = Game::LANE1;
    constexpr float lane2_spawn = Game::LANE2;
    constexpr float lane3_spawn = Game::LANE3;
    constexpr float lane4_spawn = Game::LANE4;
    constexpr float move_time = Game::NOTE_TIME;
    return Game::Rhythm::NoteField(
        spawn_level,
        judge_level,
        lane1_spawn,
        lane2_spawn,
        lane3_spawn,
        lane4_spawn,
        move_time);
}

auto sp_accent = load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
auto sp_rain = load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
auto sp_normal = load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
auto sp_disabled = load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
auto sp_hold = load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
auto sp_hold_disable = load_sprite("img/rhythm/base_hold_disabled.dds", "hold_disabled", 100, 960);
InputAttributeDescription sprite_vs_input_attributes[] = {
    InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
    InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

InputAttributeDescription sprite_ps_input_attributes[] = {
    InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
    InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};
auto sprite_vs = load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 3);
auto sprite_ps = load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 3);
auto fn = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

Math::Point Scene::field_to_point(const int lane, const Game::Rhythm::NoteField &field)
{
    if (lane == 0)
        return Math::Point{field.lane1_spawn, field.spawn_level, 0};
    if (lane == 1)
        return Math::Point{field.lane2_spawn, field.spawn_level, 0};
    if (lane == 2)
        return Math::Point{field.lane3_spawn, field.spawn_level, 0};
    if (lane == 3)
        return Math::Point{field.lane4_spawn, field.spawn_level, 0};
    return Math::Point{0, 0, 0};
}

Game::Render::Sprite Scene::assign_sprite(const int type)
{
    if (type == 1)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("accent")),
            .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 4};
    }
    if (type == 2)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("rain")),
            .pos = {{-75, 7.5, 0}, {75, 7.5, 0}, {75, -7.5, 0}, {-75, -7.5, 0}}, .layer = 5};
    }
    return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")),
        .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 4};
}

void Scene::DemoRhythm::load_chart(
    std::shared_ptr<TaskManager> &tm,
    Game::Battle::ChartData &chart,
    Game::Rhythm::NoteField &field)
{
    auto &[lanes] = chart;

    // repeat for each lane
    LOG_INFO("Loading chart...");
    for (auto &lane: lanes)
    {
        while (lane.current_note < lane.notes.size())
        {
            auto &note = lane.notes.at(lane.current_note);
            auto pos = field_to_point(lane.lane_number, field);

            if (note.is_hold)
            {
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing},
                        HoldStart{true},
                        NoteType{note.note_type},
                        NoteStatus{0},
                        assign_sprite(note.note_type),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing_end},
                        HoldStart{false},
                        NoteType{-1},
                        NoteStatus{0},
                        assign_sprite(-1),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
                tm->create_entity<
                    Game::Rhythm::HoldConnect, NoteStatus,
                    Game::Render::Sprite,
                    Game::Render::Material,
                    Game::Render::Transform>(
                        Game::Rhythm::HoldConnect{lane.lane_number, note.timing, note.timing_end},
                        NoteStatus{0},
                        Game::Render::Sprite{.sp = sp_hold, .pos = {{-25, 0, 0}, {25, 0, 0}, {25, 0, 0}, {-25, 0, 0}},
                        .layer = 0, .u0 = 0.0f, .v0 = 0.0f, .u1 = 1.0f, .v1 = 0.0f},
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
            }
            else
            {
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing},
                        HoldStart{false},
                        NoteType{note.note_type},
                        NoteStatus{0},
                        assign_sprite(note.note_type),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
            }

            ++lane.current_note;
        }
    }
    LOG_INFO("Finishing loading chart")
}

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

std::shared_ptr<Scene::DemoRhythm::TaskManager> Scene::DemoRhythm::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::RhythmState,
    Game::Battle::ChartData,
    Game::Audio::SoundRegistry>
    (
        create_battle_state(),
        create_rhythm_state(),
        create_demo_chart(),
        Game::Audio::init_sounds());

    // InputManager
    tm->create_entity<Game::Input>(Game::Input());

    // tm->create_entity<Game::Battle::LevelData>(create_level1_chartdata());

    // Create Lane
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    tm->create_entity<Game::Rhythm::NoteField>(create_field());

    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = sp_normal, .pos = {{-500, 5, 0}, {500, 5, 0}, {500, -5, 0}, {-500, -5, 0}}},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * -2 / 3, 0}, 0, 0, 0});

    tm->create_entity<Game::Rhythm::JudgeText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::JudgeText(),
        Game::Render::Text{.font = fn, .text = "", .layer = 5},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 2 / 3, 0}, 0, 0, 0});

    auto chart = create_demo_chart();
    auto field = create_field();

    load_chart(tm, chart, field);

    tm->run_all();
    return (tm);
}

Scene::DemoRhythm::ResourceManager Scene::DemoRhythm::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return ResourceManager();
}
