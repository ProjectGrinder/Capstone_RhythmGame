#pragma once

extern "C" long double get_delta_time();

namespace Game::Test
{
    struct FpsCounter
    {
        int frames = 0;
        long double delta_count = 0;
    };

    template <typename T>
    void fps_counter([[maybe_unused]] T &syscall, System::ECS::Query<FpsCounter, Render::Text> &query)
    {
        if (query.begin() == query.end())
            return;

        auto &fps_counter = query.front().get<FpsCounter>();
        auto &text = query.front().get<Render::Text>();

        fps_counter.delta_count += get_delta_time();
        fps_counter.frames++;

        if (fps_counter.delta_count >= 1000.0L)
        {
            text.text = std::to_string(fps_counter.frames);
            fps_counter.frames = 0;
            fps_counter.delta_count = 0;
        }
    }

}