#pragma once
namespace Game::Render
{
    struct Resize
    {
        Math::Point scale;
        int time;
        int current_time = 0;
        Resize(const Math::Point _scale, const int _time = 1000) : scale(_scale), time(_time) {}
    };
}