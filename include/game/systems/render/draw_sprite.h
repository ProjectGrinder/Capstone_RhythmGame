#pragma once
#include "draw_triangle.h"
#include "game/components/render/sprite.h"
#include "system.h"

namespace Game::Render
{
    template<typename T>
    void draw_sprite([[maybe_unused]] T &syscall, System::ECS::Query<Sprite, Material, Transform> &query)
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
            intent.common.info.sp_id = ASSET_INDEX(sprite.sp->id);

            const bool flipX = sprite.flipX ^ (tra.scaleX < 0);
            const bool flipY = sprite.flipY ^ (tra.scaleY<0);
            const Math::Point scale_point(Math::Point(tra.scaleX, tra.scaleY,1));

            intent.special.sprite = System::Render::SpriteDrawDesc{
                    .points = {sprite.pos[0] * scale_point, sprite.pos[1] * scale_point, sprite.pos[2] * scale_point, sprite.pos[3] * scale_point},
                    .color = sprite.color,
                    .u0 = sprite.u0,
                    .v0 = sprite.v0,
                    .u1 = sprite.u1,
                    .v1 = sprite.v1,
                    .flipX = flipX,
                    .flipY = flipY};
        }
    }
} // namespace Game::Render
