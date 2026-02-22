#include "system.h"
#include "utils/print_debug.h"

namespace System::Render
{

    typedef void *CompositorHandler;
    extern "C" CompositorHandler get_compositor();

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
            switch (drawIntent.kind)
            {
            case DrawKind::KIND_SPRITE: {
                auto &sprite_draw_desc = std::get<SpriteDrawDesc>(drawIntent.special);
                auto &dst_rect = sprite_draw_desc.dst_rect;
                if (auto composed_dst = Math::project_rect_world_to_ndc(dst_rect, camera);
                    Math::ndc_in_camera(composed_dst))
                {
                    compositor._items.push_back(
                            CompositorItem{
                                    .kind = DrawKind::KIND_SPRITE,
                                    .common = drawIntent.common,
                                    .special = ComposedSpriteDesc{
                                            sprite_draw_desc.texture,
                                            sprite_draw_desc.src_rect,
                                            std::move(composed_dst)}});
                }
                break;
            }
            case DrawKind::KIND_TEXT: {
                auto &text_draw_desc = std::get<TextDrawDesc>(drawIntent.special);
                compositor._items.push_back(
                        CompositorItem{
                                .kind = DrawKind::KIND_TEXT,
                                .common = drawIntent.common,
                                .special = ComposedTextDesc{
                                        text_draw_desc.text,
                                        text_draw_desc.font_name,
                                        text_draw_desc.font_size,
                                        Math::project_text_anchor_world_to_ndc(text_draw_desc, camera),
                                        Math::Vector2<float>{text_draw_desc.anchor_x, text_draw_desc.anchor_y}}});
                break;
            }
            default: {
                continue;
            }
            }
        }
    }
} // namespace System::Render
