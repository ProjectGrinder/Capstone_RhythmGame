#pragma once

namespace Game::World
{
    struct CameraAttractor
    {
        float lifetime;
        int order;
        CameraAttractor(const float lifetime, const int order = 0) : lifetime(lifetime), order(order) {}
    };
} // namespace Game::Overview
