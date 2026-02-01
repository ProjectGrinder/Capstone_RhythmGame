#pragma once

#include <vector>
#include <optional>
#include <variant>

namespace System
{
    enum class DrawKind : uint8_t
    {
        Sprite,
        Text,
    };

    struct Color
    {
        float r,g,b,a;
    };

    struct Rect
    {
        float u0,v0,u1,v1;
    };

    struct DrawCommon
    {
        // Pipeline selection (names/ids, not GPU objects)
        const char *vert_shader = nullptr;
        const char *pixel_shader = nullptr;

        // Render intent
        bool visible = true;
        /* render_prior: coarse render priority; larger values are drawn later (on top). */
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

        // High-level text
        std::string_view text{};
        const char *font_name = nullptr;
        uint32_t font_size = 0;
    };

    struct DrawIntent
    {
        DrawKind kind{};
        std::variant<SpriteDrawDesc, TextDrawDesc> payload{};
    };

    class IntentStorage
    {
        // TODO: Change storage from any to DrawDescription
        std::vector<std::optional<DrawIntent>> _storage;

        static size_t alloc_slot();
        static void free_slot(size_t slot);
        static IntentStorage &instance();
    };
}