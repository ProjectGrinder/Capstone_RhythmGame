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
