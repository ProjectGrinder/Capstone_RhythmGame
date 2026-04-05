#pragma once

#include "game/components/render/text.h"
#include "system.h"

namespace Game::Render
{
    template<typename T>
    void text_intent([[maybe_unused]] T &syscall, System::ECS::Query<Text> &query)
    {
        // Rewrite lmaoo
        (void) query;
    }
} // namespace Game::Render

