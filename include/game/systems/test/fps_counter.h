#pragma once

extern "C" long double get_delta_time();

namespace Game::Test
{
    struct FpsCounter
    {
        static constexpr size_t WINDOW_SIZE = 60;
        long double frame_times[WINDOW_SIZE] = {0};
        size_t current_index = 0;
    };

    template<typename T>
    void fps_counter([[maybe_unused]] T &syscall, System::ECS::Query<FpsCounter, Render::Text> &query)
    {
        if (query.begin() == query.end())
            return;

        auto &fps = query.front().get<FpsCounter>();
        auto &text = query.front().get<Render::Text>();

        long double dt = get_delta_time();
        fps.frame_times[fps.current_index] = dt;
        fps.current_index = (fps.current_index + 1) % FpsCounter::WINDOW_SIZE;

        if (fps.current_index % 10 == 0)
        {
            long double avg_dt = 0;
            for (auto t: fps.frame_times)
                avg_dt += t;
            avg_dt /= FpsCounter::WINDOW_SIZE;

            int current_fps = (avg_dt > 0) ? static_cast<int>(1000.0L / avg_dt) : 0;

            text.text = std::to_string(current_fps) + " FPS (" + std::to_string(avg_dt).substr(0, 4) + "ms)";
        }
    }

} // namespace Game::Test
