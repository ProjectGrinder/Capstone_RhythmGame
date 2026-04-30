#pragma once
namespace Game::Render
{
    struct Flicker
    {
        float init_alpha =UNASSIGNED;
        bool flicker = false;
        int time;
        int flicker_rate;
        int flicker_delay = 0;
        Flicker(const int time = 5000, const int flicker_rate = 100) : time(time), flicker_rate(flicker_rate) {}
    };
}