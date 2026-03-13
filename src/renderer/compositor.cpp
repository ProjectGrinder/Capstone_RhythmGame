#include "system/compositor.h"
#include <variant>
#include "../assets_manager/assets_manager.h"
#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *CompositorHandler;
    extern "C"
    {
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
            if (drawIntent.common.pixel_shader != nullptr)
            {
                if (!has_assets(drawIntent.common.pixel_shader))
                {
                    // TODO: Encode InputAttributeDescription and Count
                    common.pixel_shader = load_pixel_shader(
                            drawIntent.common.pixel_shader,
                            drawIntent.common.pixel_shader,
                            drawIntent.common.pixel_shader_input_attributes.data(),
                            drawIntent.common.pixel_shader_input_attributes_count);
                }
                else
                {
                    common.pixel_shader = get_assets_id(drawIntent.common.pixel_shader);
                }
            }
            else
            {
                common.pixel_shader = static_cast<assets_id>(-1);
            }

            if (drawIntent.common.vert_shader != nullptr)
            {
                if (!has_assets(drawIntent.common.vert_shader))
                {
                    // TODO: Encode InputAttributeDescription and Count
                    common.vert_shader = load_vertex_shader(
                            drawIntent.common.vert_shader,
                            drawIntent.common.vert_shader,
                            drawIntent.common.vert_shader_input_attributes.data(),
                            drawIntent.common.vert_shader_input_attributes_count);
                }
                else
                {
                    common.vert_shader = get_assets_id(drawIntent.common.vert_shader);
                }
            }
            else
            {
                common.vert_shader = static_cast<assets_id>(-1);
            }

            common.render_prior = drawIntent.common.render_prior;
            common.color = drawIntent.common.color;
            common.layer = drawIntent.common.layer;
            common.order = drawIntent.common.order;

            // rotate on global
            const auto pivot = drawIntent.common.pivot;
            const auto rotation_z = drawIntent.common.rotation_z;

            std::variant<ComposedSpriteDesc, ComposedTextDesc, TriangleDrawDesc> special{};
            switch (drawIntent.kind)
            {
            case DrawKind::KIND_SPRITE: {
                auto &sprite_draw_desc = std::get<SpriteDrawDesc>(drawIntent.special);
                special = ComposedSpriteDesc{};
                auto &sprite = std::get<ComposedSpriteDesc>(special);
                sprite.texture = get_assets_id(sprite_draw_desc.texture);
                if (sprite.texture == (uint32_t) -1)
                {
                    sprite.texture = load_sprite(sprite_draw_desc.texture, sprite_draw_desc.texture, 0, 0);
                }
                sprite.src_rect = sprite_draw_desc.src_rect;
                auto dst_rect = Math::project_rect_local_to_ndc(sprite_draw_desc.dst_rect, rotation_z, pivot, camera);
                sprite.dst_rect[0] = dst_rect.at(0);
                sprite.dst_rect[1] = dst_rect.at(1);
                sprite.dst_rect[2] = dst_rect.at(2);
                sprite.dst_rect[3] = dst_rect.at(3);
                sprite.flipX = sprite_draw_desc.flipX;
                sprite.flipY = sprite_draw_desc.flipY;
                break;
            }
            case DrawKind::KIND_TEXT: {
                /*
                auto &text_draw_desc = std::get<TextDrawDesc>(drawIntent.special);
                special = ComposedTextDesc{};
                auto &text = std::get<ComposedTextDesc>(special);
                // text, fontname, fontsize, position, anchor
                text.text = text_draw_desc.text;
                text.font = get_assets_id(text_draw_desc.font_name);
                if (text.font == (uint32_t) -1)
                {
                    text.font = load_font(text_draw_desc.font_name, text_draw_desc.font_name, 0);
                }
                */
                break;
            }
            case DrawKind::KIND_UNKNOWN: {
                break;
            }
            case DrawKind::KIND_TRIANGLE:
                auto &triangle_draw_desc = std::get<TriangleDrawDesc>(intent.value().special);
                special = TriangleDrawDesc{};
                auto &triangle = std::get<TriangleDrawDesc>(special);
                triangle = Math::local_to_ndc(triangle_draw_desc, rotation_z, pivot, camera);
                break;
            }
            compositor._items.push_back(CompositorItem{drawIntent.kind, common, std::move(special)});
        }
        auto intent_size = intents.size();
        auto items_size = compositor._items.size();
        LOG_INFO("Composed %d intents into %d items", intent_size, items_size);
    }
} // namespace System::Render
