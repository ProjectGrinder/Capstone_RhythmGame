#pragma once
#include <system/asset_manager.h>
#include "maths/color.h"

#pragma once

namespace Game::Render
{
    struct Material
    {

        const AssetsRecord *vs = nullptr;
        const AssetsRecord *ps = nullptr;

        bool visible = true;
        uint32_t render_prior = 0;
        Math::Color color{};

        Material(const AssetsRecord *vs, const AssetsRecord *ps, const Math::Color color = Math::Color(), const uint32_t prior = 0) :
            vs(vs), ps(ps), render_prior(prior), color(color)
        {}
    };
} // namespace Game::Render
