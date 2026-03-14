#pragma once
#include "maths.h"

namespace Game::Render
{
    struct Triangle
    {
        Math::Point points[3];
        uint32_t layer;     // Layer to render
        uint32_t order;     // Order of render in layer
    };
} // namespace Game::Render