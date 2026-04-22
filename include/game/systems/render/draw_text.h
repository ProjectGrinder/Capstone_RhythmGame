#pragma once

#include "game/components/render/text.h"
#include "game/components/render/material.h"
#include "game/components/render/transform.h"
#include "system/asset_manager.h"
#include "system/intent_storage.h"

namespace Game::Render
{
    template<typename T>
    void draw_text([[maybe_unused]] T &syscall, System::ECS::Query<Text, Material, Transform> &query)
    {
        for (auto &[id, comps] : query)
        {
            const auto &[font, text, color, layer, order] = comps.get<Text>();
            const auto &material = comps.get<Material>();
            const auto &tra = comps.get<Transform>();

            const auto glyphs_data = font->info.info.as_font.data;
            const auto glyphs_count = font->info.info.as_font.count;

            // in local space, the "pen" starts at (0,0,0).
            auto pen = Math::Point(0,0,0);

            for (char c : text)
            {
                int i = 0;
                while (i < (int)glyphs_count)
                {
                    if (glyphs_data[i].character == c)
                    {
                        break;
                    }
                    i++;
                }

                if (i == (int)glyphs_count) {continue;} // cannot render, no glyph data
                const auto glyph = &glyphs_data[i];
                const auto u0 = glyph->u0;
                const auto v0 = glyph->v0;
                const auto u1 = glyph->u1;
                const auto v1 = glyph->v1;
                const auto width = glyph->width;
                const auto height = glyph->height;
                const auto bearing_x = glyph->bearing_x;
                const auto bearing_y = glyph->bearing_y;
                const auto advance = glyph->advance;

                const auto glyph_left = pen.x + bearing_x;
                const auto glyph_bottom = pen.y - height + bearing_y;
                const auto glyph_right = glyph_left + width;
                const auto glyph_top = glyph_bottom + height;

                const auto glyph_bottom_left = Math::Point{glyph_left, glyph_bottom, 0};
                const auto glyph_bottom_right = Math::Point{glyph_right, glyph_bottom, 0};
                const auto glyph_top_left = Math::Point{glyph_left, glyph_top, 0};
                const auto glyph_top_right = Math::Point{glyph_right, glyph_top, 0};

                auto &intent = System::Render::IntentStorage::allocate_packed();
                intent.kind = System::Render::DrawKind::KIND_SPRITE;

                Helpers::fill_common(intent, material, tra, (uint8_t) layer);

                intent.common.sp = font;
                intent.common.info.sp_id = ASSET_INDEX(font->id);

                intent.special.sprite = System::Render::SpriteDrawDesc{
                .points = {glyph_top_left, glyph_top_right, glyph_bottom_right, glyph_bottom_left},
                .color = color,
                .u0 = u0,
                .v0 = v0,
                .u1 = u1,
                .v1 = v1,
                .flipX = false,
                .flipY = false};

                pen.x += advance;
            }
        }
    }
} // namespace Game::Render

