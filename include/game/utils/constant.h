#pragma once
#include "../../src/windows/windows_types.h"

#define PI 3.1415927f

namespace Game
{
    extern "C" Window get_window_size();
    const float HALF_HEIGHT = static_cast<float>(get_window_size().height) / 2;
    const float HALF_WIDTH = static_cast<float>(get_window_size().width) / 2;
    const float BOX_SIZE = static_cast<float>(get_window_size().width) / 6;
    constexpr Math::Point init_player_pos = {0,-240};

    constexpr float NOTE_WIDTH = 150.0f;
    constexpr float NOTE_HEIGHT = 30.0f;
    const float JUDGE_LEVEL = HALF_HEIGHT * -2 / 3;
    // x-value of 4 lanes
    constexpr float LANE1 = -1 * (NOTE_WIDTH * 1.65f);
    constexpr float LANE2 = -1 * (NOTE_WIDTH * 0.55f);
    constexpr float LANE3 = NOTE_WIDTH * 0.55f;
    constexpr float LANE4 = NOTE_WIDTH * 1.65f;

    constexpr float NOTE_TIME = 4000.00f; // default note move time at speed 1

    const Math::Point BOX_BH_POS[4] = {
        {-BOX_SIZE,BOX_SIZE,0},
        {BOX_SIZE,BOX_SIZE,0},
        {BOX_SIZE,-BOX_SIZE,0},
        {-BOX_SIZE,-BOX_SIZE,0}
};
    // const Math::Point BOX_RG_POS[4] = {
    //     {-BOX_SIZE+HALF_HEIGHT/4,BOX_SIZE+HALF_HEIGHT/2,0},
    //     {BOX_SIZE-HALF_HEIGHT/4,BOX_SIZE+HALF_HEIGHT/2,0},
    //     {BOX_SIZE+HALF_HEIGHT/8,-BOX_SIZE-HALF_HEIGHT/4,0},
    //     {-BOX_SIZE-HALF_HEIGHT/8,-BOX_SIZE-HALF_HEIGHT/4,0}
    // };

    const Math::Point BOX_RG_POS[4] = {
        {-BOX_SIZE-HALF_WIDTH/22,BOX_SIZE+HALF_HEIGHT*5/4,0},
        {BOX_SIZE+HALF_WIDTH/22,BOX_SIZE+HALF_HEIGHT*5/4,0},
        {BOX_SIZE+HALF_WIDTH/22,-BOX_SIZE-HALF_HEIGHT/4,0},
        {-BOX_SIZE-HALF_WIDTH/22,-BOX_SIZE-HALF_HEIGHT/4,0}
    };

    // for judgement line (half)
    constexpr float JUDGE_LINE_WIDTH = 350.00f;
    constexpr float JUDGE_LINE_HEIGHT = 5.00f;
}