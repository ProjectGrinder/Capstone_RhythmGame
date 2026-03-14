#pragma once

extern "C" long double get_delta_time();

namespace Game::Render
{
    template<typename T>
    void spin_camera([[maybe_unused]] T &syscall, System::ECS::Query<Game::Render::Camera2D> &query)
    {
        if (query.begin() == query.end())
            return;

        query.front().get<Game::Render::Camera2D>().rotation += static_cast<float>(0.001f * 1.0f * get_delta_time());
    }
} // namespace Game::Render
