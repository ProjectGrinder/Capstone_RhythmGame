#pragma once

namespace Game::Render
{
    struct Camera2D {
        // For setting up view/prep Matrix
        float offsetX, offsetY; // Camera View Offset (Position)
        float scaleX, scaleY; // Camera View Size
        float rotation; // Camera rotation
        float zoom; // Camera zoom
    };
}
