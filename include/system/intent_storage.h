#pragma once

#include <array>
#include <optional>
#include <variant>
#include <vector>

#include "maths.h"
#include "maths/color.h"
#include "system/asset_manager.h"
#include "utils/input_attribute_description.h"

using assets_id = uint32_t;

namespace System::Render
{
    // Common
    enum class DrawKind : uint8_t
    {
        KIND_UNKNOWN = 0,
        KIND_SPRITE,
        KIND_TEXT,
        KIND_TRIANGLE,
    };

    struct Color
    {
        float r, g, b, a;
        Color() : r(0), g(0), b(0), a(0)
        {}
        explicit Color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a)
        {}
        Color(Color &) noexcept = default;
        Color(const Color &) noexcept = default;
        Color &operator=(const Color &) noexcept = default;
    };

    struct Rect
    {
        float u0, v0, u1, v1;
    };

    struct DrawCommon
    {
        const AssetsRecord *vs = nullptr;
        const AssetsRecord *ps = nullptr;
        const AssetsRecord *sp = nullptr;

        union
        {
            struct
            {
                uint16_t sp_id;
                uint16_t ps_id;
                uint16_t vs_id;
                uint8_t padding;
                uint8_t layer;
            } info;
            uint64_t key;
        };

        Color color{};

        Math::Point pivot;
        float rotation_z = 0.0f;
        bool visible = true;
    };

    struct SpriteDrawDesc
    {
        Math::Point points[4]{};
        Math::Color color{};
        float u0 = 0.0f;
        float v0 = 0.0f;
        float u1 = 1.0f;
        float v1 = 1.0f;
        bool flipX = false;
        bool flipY = false;
    };

    struct TextDrawDesc
    {
        std::string_view text{};
        const char *font_name = nullptr;
        uint32_t font_size = 0;

        float x = 0.0f;
        float y = 0.0f;
        float anchor_x = 0.5f;
        float anchor_y = 0.5f;
    };

    struct TriangleDrawDesc
    {
        Math::Point points[3];
    };

    struct DrawIntent
    {
        DrawKind kind{DrawKind::KIND_UNKNOWN};
        DrawCommon common{};
        std::variant<SpriteDrawDesc, TextDrawDesc, TriangleDrawDesc> special{};
        uint32_t version = 0;
    };

    // Render Packet

    struct RenderSortKey
    {
        uint64_t key;
    };

    struct Camera
    {
        Math::Point offset;
        float scaleX, scaleY;
        float rotation;
        float zoom;
        uint32_t version = 0;
    };

    class IntentStorage
    {
    private:
        std::vector<DrawIntent> _intent_storage;
        static constexpr size_t NUM_BUFFERS = 3;
        std::array<std::vector<DrawIntent>, NUM_BUFFERS> _packed_buffers;

        std::array<size_t, NUM_BUFFERS> _buffer_counts{0, 0, 0};

        uint32_t _current_frame_idx = 0;
        Camera _camera{};

    public:
        IntentStorage()
        {
            for (auto &buf: _packed_buffers)
                buf.reserve(4096);
            _intent_storage.reserve(1024);
        }

        static IntentStorage &instance();

        static size_t alloc_slot();
        static void free_slot(size_t slot);
        static DrawIntent &get_intent(size_t slot);
        static const std::vector<DrawIntent> &get_intents();

        static void next_frame();
        static DrawIntent &allocate_packed();

        static const std::vector<DrawIntent> &get_packed_for_render(uint32_t render_frame_idx);
        static Camera &get_camera()
        {
            return instance()._camera;
        }
    };
} // namespace System::Render
