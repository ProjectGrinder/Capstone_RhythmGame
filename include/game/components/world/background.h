#pragma once

namespace Game::World
{
    struct Background
    {
        float pan_value = 0.f; // in case of parallax effect
        Background() = default;
        explicit Background(const float pan_value) : pan_value(pan_value) {}
    };
}