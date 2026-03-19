#pragma once

extern "C" long double get_delta_time();

namespace Game::Render
{
    template<typename T>
    void spin_sprite([[maybe_unused]] T &syscall, System::ECS::Query<Game::Render::Sprite, Game::Render::Transform, Physics::Rotation> &query)
    {
        for (auto &entry: query)
        {
            auto &transform = entry.get<Game::Render::Transform>();
            auto &rotation = entry.get<Physics::Rotation>();
            transform.angleZ += static_cast<float>(rotation.angleZ * 1.0f * get_delta_time());
        }
    }
} // namespace Game::Render
