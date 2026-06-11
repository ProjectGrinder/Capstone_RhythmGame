#pragma once
namespace Game::Render
{
    struct Flicker
    {
        float init_alpha;
        bool flicker = false;
        int time;
        int flicker_rate;
        int flicker_delay = 0;
        Flicker(const float init_alpha = -1, const int time = 5000, const int flicker_rate = 100) : init_alpha(init_alpha), time(time), flicker_rate(flicker_rate) {}
    };
}