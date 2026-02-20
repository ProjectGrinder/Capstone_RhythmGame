#pragma once

namespace Game::Render
{
    template<typename T>
    void set_camera([[maybe_unused]] T &syscall, System::ECS::Query<Game::Render::Camera2D> &query)
    {
        const auto &[offsetX, offsetY, scaleX, scaleY, rotation, zoom] = query.front().get<System::Render::Camera>();
        // ReSharper disable once CppUseStructuredBinding
        auto &intent_camera = System::Render::IntentStorage::get_camera();
        intent_camera.offsetX = offsetX;
        intent_camera.offsetY = offsetY;
        intent_camera.scaleX = scaleX;
        intent_camera.scaleY = scaleY;
        intent_camera.zoom = zoom;
        intent_camera.rotation = rotation;
    }
} // namespace Game::Render
