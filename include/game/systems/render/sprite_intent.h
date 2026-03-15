#pragma once
#include "game/components/render/intent_handle.h"
#include "game/components/render/sprite.h"
#include "system.h"

namespace Game::Render
{
    template<typename T>
    void sprite_intent([[maybe_unused]] T &syscall, System::ECS::Query<Sprite, IntentHandle> &query)
    {
        // Rewrite lmao
        (void) query;
    }
} // namespace Game::Render
