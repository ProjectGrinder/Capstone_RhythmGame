#pragma once

#include "game/systems/render/draw_triangle.h"
#include "system.h"

namespace Game::Rhythm
{
    template<typename T>
    void draw_notes(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Render::Sprite, Render::Material, Render::Transform, NoteStatus> &query)
    {
        for (auto &[id, comp]: query)
        {
            if (comp.get<NoteStatus>().state != 1)
                continue;

            const auto &material = comp.get<Render::Material>();
            if (!material.visible)
                continue;

            const auto &sprite = comp.get<Render::Sprite>();
            const auto &tra = comp.get<Transform>();

            auto &intent = System::Render::IntentStorage::allocate_packed();
            intent.kind = System::Render::DrawKind::KIND_SPRITE;

            Render::Helpers::fill_common(intent, material, tra, (uint8_t) sprite.layer);
            intent.common.sp = sprite.sp;
            intent.common.info.sp_id = ASSET_INDEX(sprite.sp->id);

            intent.special = System::Render::SpriteDrawDesc{
                .points = {sprite.pos[0], sprite.pos[1], sprite.pos[2], sprite.pos[3]},
                .u0 = sprite.u0,
                .v0 = sprite.v0,
                .u1 = sprite.u1,
                .v1 = sprite.v1,
                .flipX = sprite.flipX,
                .flipY = sprite.flipY};
        }
    }
} // namespace Game::Render