#pragma once

namespace Game::Render
{
    struct Camera2D
    {
        // For setting up view/prep Matrix
        float offsetX = 0, offsetY = 0; // Camera View Offset (Position)
        float scaleX = 1, scaleY = 1;   // Camera View Size
        float rotation = 0;         // Camera rotation
        float zoom = 1;             // Camera zoom
    };
} // namespace Game::Render
