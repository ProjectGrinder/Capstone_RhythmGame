#pragma once
#include "../../src/windows/windows_types.h"
namespace Game
{
    extern "C" Window get_window_size();
    const float HALF_HEIGHT = static_cast<float>(get_window_size().height) / 2;
    const float HALF_WIDTH = static_cast<float>(get_window_size().width) / 2;
    const float BOX_SIZE = static_cast<float>(get_window_size().width) / 6;
    constexpr Math::Point init_player_pos = {0,-240};

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
        {-BOX_SIZE-HALF_HEIGHT/8,BOX_SIZE+HALF_HEIGHT*9/8,0},
        {BOX_SIZE+HALF_HEIGHT/8,BOX_SIZE+HALF_HEIGHT*9/8,0},
        {BOX_SIZE+HALF_HEIGHT/8,-BOX_SIZE-HALF_HEIGHT/4,0},
        {-BOX_SIZE-HALF_HEIGHT/8,-BOX_SIZE-HALF_HEIGHT/4,0}
    };
}