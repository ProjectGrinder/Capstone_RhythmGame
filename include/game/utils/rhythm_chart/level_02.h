#pragma once

inline Game::Battle::ChartData level_02_light()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    chart.difficulty.difficulty = Game::Battle::LIGHT;
    chart.difficulty.level = 2;

    // RESPONSE 1
    chart.lanes[0].notes.emplace_back(false, 29647, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 31059, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 32471, 33176, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 33176, 33882, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 33882, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 34588, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 35294, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 36000, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 39529, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 40059, 40588, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 40941, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 41471, 42000, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 42353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 43059, 43765, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 43765, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 44118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 44471, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 45176, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 45706, 46235, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 47118, 47647, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 48000, 48353, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 48706, 49059, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);

    // RESPONSE 2
    chart.lanes[0].notes.emplace_back(true, 74824, 75353, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 75353, 75881, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 76235, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 76941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 77647, 78353, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 77647, 78353, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 79059, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 79588, 80118, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 80471, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 81000, 81529, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 81882, 82588, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 82588, 84706, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 83294, 84706, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 87529, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 87529, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 88941, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 88941, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 89647, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 89647, 0, Game::Battle::ACCENT);

    // RESPONSE 3
    chart.lanes[2].notes.emplace_back(false, 112941, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 114353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 115059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 115765, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 115765, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 116471, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 116471, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 117176, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 118588, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 120000, 120706, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 121412, 122118, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 121412, 122118, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 122824, 123529, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 123529, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 124235, 124941, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 124941, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 125647, 126353, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 126353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 127059, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 127059, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 127765, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 127765, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 128471, 129176, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 129176, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 129882, 130588, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 130588, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 131294, 132000, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 132000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 132706, 133412, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 132706, 133412, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 134118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 134118, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);

    return (chart);
}

inline Game::Battle::ChartData level_02_spark()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    chart.difficulty.difficulty = Game::Battle::SPARK;
    chart.difficulty.level = 4;

    // RESPONSE 1
    chart.lanes[0].notes.emplace_back(false, 29647, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 30353, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 31059, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 31765, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 32471, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 32471, 33176, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 33176, 33882, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 33882, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 34588, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 34941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 35294, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 36000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 36353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 38824, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 38824, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 39529, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 39794, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 40059, 40588, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 40941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 41206, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 41471, 42000, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 42353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 42706, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 43059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 43412, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 43765, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 44118, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 44471, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 45176, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 45441, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 45706, 46235, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 46588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46853, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 47118, 47647, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 48000, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 48353, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 48706, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 49059, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);

    // RESPONSE 2
    chart.lanes[0].notes.emplace_back(true, 74471, 74824, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 74824, 75353, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 75353, 75882, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 75882, 76235, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 76235, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 76941, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 76941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 77647, 78353, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 77647, 78353, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 79059, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 79324, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 79588, 80118, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 80471, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 80735, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 81000, 81529, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 81882, 82588, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 82588, 82941, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 82941, 84706, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 83294, 84706, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 83647, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 83824, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 84000, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 84176, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 84353, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 84529, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 87529, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 87529, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 88235, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 88235, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 88941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 88941, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 89647, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 89647, 0, Game::Battle::NORMAL);

    // RESPONSE 3
    chart.lanes[3].notes.emplace_back(false, 112941, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 113647, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 114000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 114353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 115059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 115412, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 115765, 116118, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 115765, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 116471, 116824, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 116471, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 117176, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 117176, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 117882, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 118235, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 118588, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 119294, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 119647, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 120000, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 120000, 120706, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 120706, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 121059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 121412, 122118, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 121412, 122118, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 121676, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 121676, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 121941, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 121941, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 122824, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 123176, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 123529, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 124235, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 124588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 124941, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 125647, 126353, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 125647, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 126353, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 126706, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 127059, 127412, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 127059, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 127765, 128118, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 127765, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 128471, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 128824, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 129176, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 129882, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 130235, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 130588, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 131294, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 131294, 132000, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 132000, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 132353, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 132706, 133412, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 132706, 133412, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 134118, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 134382, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 134647, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);

    return (chart);
}

inline Game::Battle::ChartData level_02_blaze()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    chart.difficulty.difficulty = Game::Battle::BLAZE;
    chart.difficulty.level = 5;

    // RESPONSE 1
    chart.lanes[0].notes.emplace_back(false, 29647, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 30353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 30706, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 31059, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 31412, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 31765, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 32118, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 32471, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 32471, 33176, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 33176, 33529, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 33265, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 33353, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 33441, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(true, 33529, 33882, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 33618, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33706, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 33794, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 33882, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 34235, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 34588, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 34588, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 34941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 35294, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 35294, 36000, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 35647, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 36000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 36353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 36706, 38118, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 38118, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 38206, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 38294, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 38382, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 38471, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 38559, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 38647, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 38735, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 39529, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 39794, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 40059, 40588, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 40412, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 40588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 40941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 41206, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 41471, 42000, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 41824, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 42000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 42353, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 42353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 42706, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 43059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 43412, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 43765, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 43765, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 44118, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 44118, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 44471, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 44471, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 45176, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 45441, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 45706, 46235, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 46059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46235, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 46588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 46853, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 47118, 47647, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 47471, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 47647, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 48000, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 48000, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 48353, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 48706, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 49059, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 49412, 0, Game::Battle::ACCENT);

    // RESPONSE 2
    chart.lanes[0].notes.emplace_back(true, 74471, 74824, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 74824, 75353, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 75353, 75882, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 75882, 76235, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 76235, 76412, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 76412, 76588, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 76588, 76765, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 76765, 76941, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 76941, 77118, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 77118, 77294, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 77294, 77471, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 77471, 77647, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 77647, 78353, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 77824, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 77912, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 78000, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 78088, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 78176, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 78265, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 78353, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 78706, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 78706, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 79059, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 79324, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 79588, 80118, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 79941, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 80118, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 80471, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 80735, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 81000, 81529, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 81353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 81529, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 81882, 82588, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 82059, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 82235, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 82412, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(true, 82588, 82765, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 82765, 82941, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 82941, 83118, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 83118, 83294, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 83294, 84706, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 83471, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 83559, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 83647, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 83735, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 83824, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 83912, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 84000, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 84088, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 84176, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 84265, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 84353, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 84441, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 84529, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 84618, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 84706, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 86118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 87353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 87529, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 87529, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 88235, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 88235, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 88588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 88941, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 88941, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 89294, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 89647, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 89647, 0, Game::Battle::NORMAL);

    // RESPONSE 3
    chart.lanes[0].notes.emplace_back(false, 112941, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 113294, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 113647, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 114000, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 114353, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 114353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 114706, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 115059, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 115412, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 115765, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 115765, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 116118, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 116118, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 116471, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 116471, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 116824, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 116824, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 117176, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 117176, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 117882, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 118235, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 118588, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 118941, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 119294, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 119647, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 120000, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 120000, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 120353, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 120706, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 120794, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 120882, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 120971, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 121059, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 121147, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 121235, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 121324, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 121412, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 121412, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 121676, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 121676, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 121941, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 121941, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 122118, 122471, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 122118, 122471, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 122824, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 123176, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 123529, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 123882, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 124235, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 124588, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 124941, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 125294, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 125647, 126353, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 125647, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 126353, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 126706, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 127059, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 127059, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 127412, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 127412, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 127765, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 127765, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 128118, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 128118, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 128471, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 128471, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 128824, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 129176, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 129529, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 129882, 0, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 130235, 0, Game::Battle::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 130588, 0, Game::Battle::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 130941, 0, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 131294, 131647, Game::Battle::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 131294, 132000, Game::Battle::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 132000, 0, Game::Battle::RAIN);
    chart.lanes[3].notes.emplace_back(false, 132088, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 132176, 0, Game::Battle::RAIN);
    chart.lanes[2].notes.emplace_back(false, 132265, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 132353, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(false, 132441, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 132529, 0, Game::Battle::RAIN);
    chart.lanes[0].notes.emplace_back(false, 132618, 0, Game::Battle::RAIN);
    chart.lanes[1].notes.emplace_back(true, 132706, 133412, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 132706, 133412, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 134118, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 134118, 0, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 134382, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(false, 134382, 0, Game::Battle::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 134647, 0, Game::Battle::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 134647, 0, Game::Battle::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 134824, 135529, Game::Battle::ACCENT);

    return (chart);
}

inline Game::Battle::ChartData load_level_02_chart(const int level)
{
    switch (level)
    {
    case 0:
        return level_02_light();
    case 1:
        return level_02_spark();
    case 2:
        return level_02_blaze();
    default:
        return level_02_light();
    }
}
