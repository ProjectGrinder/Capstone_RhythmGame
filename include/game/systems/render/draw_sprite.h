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
                .src_rect = {
                    .u0 = sprite.src_rect.u0,
                    .v0 = sprite.src_rect.v0,
                    .u1 = sprite.src_rect.u1,
                    .v1 = sprite.src_rect.v1,
                },
                .points = {sprite.dst_pos[0], sprite.dst_pos[1], sprite.dst_pos[2], sprite.dst_pos[3]},
                .flipX = sprite.flipX,
                .flipY = sprite.flipY
            };
        }
    }
} // namespace Game::Render
