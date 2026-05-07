#pragma once
#include "../render/draw_triangle.h"
#include "game/components/render/sprite.h"
#include "system.h"

namespace Game::Test
{
    template<typename T>
    void draw_collider([[maybe_unused]] T &syscall, System::ECS::Query<Physics::CircularCollider, Render::Transform> &circle_query, System::ECS::Query<Physics::RectangularCollider, Render::Transform> &rect_query)
    {
        const Render::Material material{
                get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))};
        const Render::Sprite circle_sp{
                get_assets_record_ptr(get_assets_id("ring16px")),
                {{-16, 16, 0}, {16, 16, 0}, {16, -16, 0}, {-16, -16, 0}},
                {0.25f,1.f,0.25f,1.f}, 4};
        const Render::Sprite rect_sp{get_assets_record_ptr(get_assets_id("Square64px")), {{-16, 16, 0}, {16, 16, 0}, {16, -16, 0}, {-16, -16, 0}}, {0.25f,1.f,0.25f,1.f}, 4};
        for (auto &[id, comp]: circle_query)
        {
            auto tra = comp.get<Render::Transform>();
            const auto &col = comp.get<Physics::CircularCollider>();

            tra.position.x += col.offset_x;
            tra.position.y += col.offset_y;
            const Math::Point scale_point(Math::Point(col.radius_x/16 * tra.scaleX,col.radius_y/16 *  tra.scaleY,1));

            auto &intent = System::Render::IntentStorage::allocate_packed();
            intent.kind = System::Render::DrawKind::KIND_SPRITE;

            Render::Helpers::fill_common(intent, material, tra, (uint8_t) circle_sp.layer);
            intent.common.sp = circle_sp.sp;
            intent.common.info.sp_id = ASSET_INDEX(circle_sp.sp->id);

            intent.special.sprite = System::Render::SpriteDrawDesc{
                    .points = {circle_sp.pos[0] * scale_point, circle_sp.pos[1] * scale_point, circle_sp.pos[2] * scale_point, circle_sp.pos[3] * scale_point},
                    .color = circle_sp.color,
                    .u0 = circle_sp.u0,
                    .v0 = circle_sp.v0,
                    .u1 = circle_sp.u1,
                    .v1 = circle_sp.v1,
                    .flipX = circle_sp.flipX,
                    .flipY = circle_sp.flipY};
        }

        for (auto &[id, comp]: rect_query)
        {
            auto tra = comp.get<Render::Transform>();
            const auto &col = comp.get<Physics::RectangularCollider>();

            tra.position.x += col.offset_x;
            tra.position.y += col.offset_y;
            const Math::Point scale_point(Math::Point(col.size_x/16 * tra.scaleX,col.size_y/16 *  tra.scaleY,1));

            auto &intent = System::Render::IntentStorage::allocate_packed();
            intent.kind = System::Render::DrawKind::KIND_SPRITE;

            Render::Helpers::fill_common(intent, material, tra, (uint8_t) rect_sp.layer);
            intent.common.sp = rect_sp.sp;
            intent.common.info.sp_id = ASSET_INDEX(rect_sp.sp->id);

            intent.special.sprite = System::Render::SpriteDrawDesc{
                .points = {rect_sp.pos[0] * scale_point, rect_sp.pos[1] * scale_point, rect_sp.pos[2] * scale_point, rect_sp.pos[3] * scale_point},
                .color = rect_sp.color,
                .u0 = rect_sp.u0,
                .v0 = rect_sp.v0,
                .u1 = rect_sp.u1,
                .v1 = rect_sp.v1,
                .flipX = rect_sp.flipX,
                .flipY = rect_sp.flipY};
        }
    }
} // namespace Game::Render
