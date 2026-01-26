
#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>

#include "game/components/render/material.h"
#include "game/components/render/rotation.h"
#include "game/components/render/sprite.h"
#include "game/components/render/text.h"
#include "game/components/render/animator.h"

namespace Game::Render
{
    enum class DrawKind : uint8_t
    {
        Sprite,
        Text,
    };

    struct DrawCommon
    {
        // Pipeline selection (names/ids, not GPU objects)
        const char *vert_shader = nullptr;
        const char *pixel_shader = nullptr;

        // Render intent
        bool visible = true;
        uint32_t render_prior = 0;

        // Per-draw parameters
        Color color{};

        // Sorting (kept here so both sprite/text can be sorted uniformly)
        uint32_t layer = 0;
        uint32_t order = 0;

        // 2D-friendly rotation hint (radians or degrees will depend on convention;
        // this merely carries the value through)
        float rotation_z = 0.0f;
    };

    struct SpriteDrawDesc
    {
        DrawCommon common{};

        const char *texture = nullptr;
        Rect src_rect{};
        Rect dst_rect{};

        bool flipX = false;
        bool flipY = false;
    };

    struct TextDrawDesc
    {
        DrawCommon common{};

        // High-level text (no glyph shaping/mesh generation here)
        std::string_view text{};
        const char *font_name = nullptr;
        uint32_t font_size = 0;
    };

    struct DrawDescription
    {
        DrawKind kind{};
        std::variant<SpriteDrawDesc, TextDrawDesc> payload{};
    };

    // Build a high-level draw description from ECS components.
    //
    // Notes:
    // - This does NOT allocate GPU objects and does NOT build RenderObject.
    // - Animator is intentionally ignored here (not "reasonable" without evaluation).
    //   The animation system should update Sprite / transform components first.
    inline std::optional<DrawDescription> BuildDrawDescription(
        const Material *material,
        const Sprite *sprite,
        const Text *text,
        const Rotation *rotation,
        const Animator *animator)
    {
        (void)animator; // evaluated elsewhere (animation system)

        // Must have material and be visible to render
        if (!material || !material->visible)
            return std::nullopt;

        DrawCommon common{};
        common.vert_shader = material->vert_shader;
        common.pixel_shader = material->pixel_shader;
        common.visible = material->visible;
        common.render_prior = material->render_prior;
        common.color = material->color;

        if (rotation)
            common.rotation_z = rotation->angleZ;

        // Try Sprite if present
        if (sprite)
        {
            SpriteDrawDesc sd{};
            sd.common = common;

            sd.common.layer = sprite->layer;
            sd.common.order = sprite->order;

            sd.texture = sprite->texture;
            sd.src_rect = sprite->src_rect;
            sd.dst_rect = sprite->dst_rect;
            sd.flipX = sprite->flipX;
            sd.flipY = sprite->flipY;

            DrawDescription out{};
            out.kind = DrawKind::Sprite;
            out.payload = sd;
            return out;
        }

        // Otherwise, try Text if it looks valid
        if (text && !text->text.empty())
        {
            TextDrawDesc td{};
            td.common = common;

            // Text currently has no layer/order; keep defaults (0)
            td.text = std::string_view(text->text);
            td.font_name = text->font_name;
            td.font_size = text->font_size;

            DrawDescription out{};
            out.kind = DrawKind::Text;
            out.payload = td;
            return out;
        }

        // No renderable component combination
        return std::nullopt;
    }
} // namespace Game::Render