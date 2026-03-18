#pragma once
#include "draw_triangle.h"
#include "game/components/render/sprite.h"
#include "system.h"

namespace Game::Render
{
    template<typename T>
    void sprite_intent([[maybe_unused]] T &syscall, System::ECS::Query<Sprite, Material, Transform> &query)
    {
        for (auto &[id, comp]: query)
        {
            const auto &material = comp.get<Material>();
            if (!material.visible)
                continue;

            const auto &sprite = comp.get<Sprite>();
            const auto &tra = comp.get<Transform>();

            auto &intent = System::Render::IntentStorage::allocate_packed();
            intent.kind = System::Render::DrawKind::KIND_SPRITE;

            Helpers::fill_common(intent, material, tra, (uint8_t) sprite.layer);
            intent.common.sp = sprite.sp;

            intent.special = System::Render::SpriteDrawDesc{
                .points = {sprite.pos[0], sprite.pos[1], sprite.pos[2], sprite.pos[3]},
                .flipX = sprite.flipX,
                .flipY = sprite.flipY
            };
        }
    }
} // namespace Game::Render
