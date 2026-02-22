
#include "system/compositor.h"
#include <variant>
#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *CompositorHandler;
    extern "C"
    {
        typedef enum
        {
            FLOAT32BITS,
            FLOAT16BITS,
            FLOAT8BITS,

            UINT32BITS,
            UINT16BITS,
            UINT8BITS,
        } InputType;

        typedef struct
        {
            char *semantic;
            InputType type;
            size_t offset;
        } InputAttributeDescription;
        typedef uint32_t assets_id;
        assets_id get_assets_id(const char *name);
        assets_id load_sprite(const char *path, const char *name, size_t width, size_t height);
        assets_id load_vertex_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count);
        assets_id load_pixel_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count);
        assets_id load_font(const char *path, const char *name, size_t size);
        int has_assets(const char *name);
        CompositorHandler get_compositor();
    }

    Compositor &Compositor::instance()
    {
        auto *instance = static_cast<Compositor *>(get_compositor());
        if (instance == nullptr)
        {
            LOG_ERROR("Compositor used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }

    void Compositor::compose(const std::vector<std::optional<DrawIntent>> &intents, const Camera &camera)
    {
        auto &compositor = instance();
        compositor._items.clear();
        for (auto &intent: intents)
        {
            if (!intent.has_value())
            {
                continue;
            }

            auto &drawIntent = intent.value();

            ComposedDrawCommon common{};
            if (!has_assets(drawIntent.common.pixel_shader))
            {
                // TODO: Encode InputAttributeDescription and Count
                common.pixel_shader =
                        load_pixel_shader(drawIntent.common.pixel_shader, drawIntent.common.pixel_shader, nullptr, 0);
            }
            else
            {
                common.pixel_shader = get_assets_id(drawIntent.common.pixel_shader);
            }

            if (!has_assets(drawIntent.common.vert_shader))
            {
                // TODO: Encode InputAttributeDescription and Count
                common.vert_shader =
                        load_vertex_shader(drawIntent.common.vert_shader, drawIntent.common.vert_shader, nullptr, 0);
            }
            else
            {
                common.vert_shader = get_assets_id(drawIntent.common.vert_shader);
            }
            common.render_prior = drawIntent.common.render_prior;
            common.color = drawIntent.common.color;
            common.layer = drawIntent.common.layer;
            common.order = drawIntent.common.order;
            common.rotation_z = drawIntent.common.rotation_z;

            std::variant<ComposedSpriteDesc, ComposedTextDesc> special{};
            switch (drawIntent.kind)
            {
            case DrawKind::KIND_SPRITE: {
                auto &sprite_draw_desc = std::get<SpriteDrawDesc>(drawIntent.special);
                special = ComposedSpriteDesc{};
                ComposedSpriteDesc &sprite = std::get<ComposedSpriteDesc>(special);
                sprite.texture = get_assets_id(sprite_draw_desc.texture);
                if (sprite.texture == -1)
                {
                    sprite.texture = load_sprite(sprite_draw_desc.texture, sprite_draw_desc.texture, 0, 0);
                }
                sprite.src_rect = sprite_draw_desc.src_rect;
                sprite.dst_rect = Math::project_rect_world_to_ndc(sprite_draw_desc.dst_rect, camera);
                if (!Math::ndc_in_camera(sprite.dst_rect))
                {
                    continue;
                }
                sprite.flipX = sprite_draw_desc.flipX;
                sprite.flipY = sprite_draw_desc.flipY;
                break;
            }
            case DrawKind::KIND_TEXT: {
                auto &text_draw_desc = std::get<TextDrawDesc>(drawIntent.special);
                special = ComposedTextDesc{};
                ComposedTextDesc &text = std::get<ComposedTextDesc>(special);
                // text, fontname, fontsize, position, anchor
                text.text = text_draw_desc.text;
                text.font = get_assets_id(text_draw_desc.font_name);
                if (text.font == -1)
                {
                    text.font = load_font(text_draw_desc.font_name, text_draw_desc.font_name, 0);
                }
                break;
            }
            case DrawKind::KIND_UNKNOWN: {
                break;
            }
            }
            compositor._items.push_back({drawIntent.kind, std::move(common), std::move(special)});
        }
    }
} // namespace System::Render
