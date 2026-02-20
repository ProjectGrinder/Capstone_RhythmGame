#pragma once

#include <optional>
#include <variant>
#include <vector>

namespace System::Render
{
    // Common
    enum class DrawKind : uint8_t
    {
        KIND_UNKNOWN = 0,
        KIND_SPRITE,
        KIND_TEXT,
    };

    struct Color
    {
        float r, g, b, a;
        Color() : r(0), g(0), b(0), a(0)
        {}
        explicit Color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a)
        {}
        Color &operator=(const Color &) noexcept = default;
    };

    struct Rect
    {
        float u0, v0, u1, v1;
    };

    // Intention

    struct DrawCommon
    {
        // Pipeline selection (names/ids, not GPU objects)
        const char *vert_shader = nullptr;
        const char *pixel_shader = nullptr;


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

        // Render intent
        bool visible = true;
    };

    struct SpriteDrawDesc
    {
        const char *texture = nullptr;
        Rect src_rect{};
        Rect dst_rect{};

        bool flipX = false;
        bool flipY = false;
    };

    struct TextDrawDesc
    {
        // High-level text
        std::string_view text{};
        const char *font_name = nullptr;
        uint32_t font_size = 0;
    };

    struct DrawIntent
    {
        DrawKind kind{DrawKind::KIND_UNKNOWN};
        DrawCommon common{};
        std::variant<SpriteDrawDesc, TextDrawDesc> special{};
    };

    // Render Packet

    struct RenderSortKey
    {
        uint32_t layer = 0;
        uint32_t render_prior = 0;
        uint32_t order = 0;
    };

    struct Camera
    {
        float offsetX, offsetY;
        float scaleX, scaleY;
        float rotation;
        float zoom;
    };

    class IntentStorage
    {
        // TODO: Change storage from any to DrawDescription
        std::vector<std::optional<DrawIntent>> _intent_storage{};
        Camera _camera{};

    public:
        static size_t alloc_slot();
        static void free_slot(size_t slot);
        static IntentStorage &instance();
        static std::optional<DrawIntent> &get_intent(size_t slot);
        static Camera &get_camera();
    };
} // namespace System::Render
