#pragma once

#define PI 3.1415927f

namespace Game
{
    // extern "C" Window get_window_size();
    constexpr float HALF_HEIGHT = 540.f;
    constexpr float HALF_WIDTH = 960.f;
    constexpr float BOX_SIZE = HALF_WIDTH / 3;
    constexpr Math::Point init_player_pos = {0,-240};

    constexpr float NOTE_WIDTH = HALF_WIDTH / 960 * 150;
    constexpr float NOTE_HEIGHT = HALF_HEIGHT / 540 * 30;
    constexpr float JUDGE_LEVEL = HALF_HEIGHT * -2 / 3;
    constexpr float SPAWN_LEVEL = HALF_HEIGHT + 20.f;
    // x-value of 4 lanes
    constexpr float LANE1 = -1 * (NOTE_WIDTH * 1.65f);
    constexpr float LANE2 = -1 * (NOTE_WIDTH * 0.55f);
    constexpr float LANE3 = NOTE_WIDTH * 0.55f;
    constexpr float LANE4 = NOTE_WIDTH * 1.65f;

    constexpr float NOTE_TIME = 4000.00f; // default note move time at speed 1

    constexpr Math::Point BOX_BH_POS[4] = {
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

    constexpr Math::Point BOX_RG_POS[4] = {
        {-BOX_SIZE-HALF_WIDTH/22,BOX_SIZE+HALF_HEIGHT*5/4,0},
        {BOX_SIZE+HALF_WIDTH/22,BOX_SIZE+HALF_HEIGHT*5/4,0},
        {BOX_SIZE+HALF_WIDTH/22,-BOX_SIZE-HALF_HEIGHT/4,0},
        {-BOX_SIZE-HALF_WIDTH/22,-BOX_SIZE-HALF_HEIGHT/4,0}
    };

    // for judgement line (half)
    constexpr float JUDGE_LINE_WIDTH = 350.00f;
    constexpr float JUDGE_LINE_HEIGHT = 5.00f;

    constexpr float CAMERA_SPEED = 0.005f;

    constexpr int IFRAME_TIME = 1500;
    constexpr float GRAZE_HITBOX_SIZE = 40.f;

    constexpr float DIALOGUE_BOX_OFFSET_Y = 80.f;
    constexpr float DIALOGUE_BOX_HEIGHT = 60.f;
    constexpr int DIALOGUE_TYPE_DELAY = 50;
}