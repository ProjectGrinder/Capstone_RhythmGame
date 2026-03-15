#pragma once
#include <system/asset_manager.h>

#pragma once

namespace Game::Render
{
    struct Color
    {
        float r, g, b, a;
        Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
        {}
        Color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a)
        {}
    };

    struct Material
    {

        const AssetsRecord *vs = nullptr;
        const AssetsRecord *ps = nullptr;

        bool visible = true;
        uint32_t render_prior = 0;
        Color color{};

        Material(const AssetsRecord *vs, const AssetsRecord *ps, uint32_t prior = 0) :
            vs(vs), ps(ps), render_prior(prior)
        {}
    };
} // namespace Game::Render
