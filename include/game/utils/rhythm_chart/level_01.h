#pragma once

inline Game::Battle::ChartData level_01_blaze()
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

inline Game::Battle::ChartData level_01_light()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    return (chart);
}

inline Game::Battle::ChartData level_01_spark() // will do later
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    return (chart);
}

inline Game::Battle::ChartData load_level_01_chart(const int level)
{
    switch (level)
    {
    case 0:
        return level_01_light();
    case 1:
        return level_01_spark();
    case 2:
        return level_01_blaze();
    default:
        return level_01_light();
    }
}