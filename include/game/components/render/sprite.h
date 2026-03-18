#pragma once
#include "maths/point.h"
#include "system/asset_manager.h"

//
namespace Game::Render
{
    // Should this be implemented somewhere else
    struct Rect
    {
        float u0, v0, u1, v1;
        Rect() : u0(0), v0(0), u1(0), v1(0)
        {}
        Rect(const float u0, const float v0, const float u1, const float v1) : u0(u0), v0(v0), u1(u1), v1(v1)
        {}
    };

    struct Sprite
    {
        const AssetsRecord *sp = nullptr;
        Math::Point pos[4]{};
        uint32_t layer = 0;     // Layer to render
        uint32_t order = 0;     // Order of render in layer
        bool flipX = false; // Flip Image in X-axis
        bool flipY = false; // Flip Image in Y-axis
    };
} // namespace Game::Render
