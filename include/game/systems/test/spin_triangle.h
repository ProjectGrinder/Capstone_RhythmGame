#pragma once

extern "C" long double get_delta_time();

namespace Game::Render
{
    template<typename T>
    void spin_triangle([[maybe_unused]] T &syscall, System::ECS::Query<Game::Render::Triangle, Game::Render::Transform> &query)
    {
        for (auto &entry: query)
        {
            auto &transform = entry.get<Game::Render::Transform>();
            transform.angleZ += static_cast<float>(0.009f * 1.0f * get_delta_time());
        }
    }
} // namespace Game::Render
