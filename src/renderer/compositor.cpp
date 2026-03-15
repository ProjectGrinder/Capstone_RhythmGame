#include "system/compositor.h"
#include <cmath>
#include <variant>
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

    void Compositor::compose(const std::vector<DrawIntent> &intents, const Camera &camera)
    {
        auto &comp = instance();

        if (intents.size() > comp._items.capacity())
        {
            comp._items.reserve(intents.size());
        }
        comp._items.clear();

        const float cam_cos = std::cos(-camera.rotation);
        const float cam_sin = std::sin(-camera.rotation);
        const float invHalfW = (camera.scaleX != 0.0f) ? (2.0f / camera.scaleX) : 0.0f;
        const float invHalfH = (camera.scaleY != 0.0f) ? (2.0f / camera.scaleY) : 0.0f;

        for (const auto &intent: intents)
        {
            if (!intent.common.visible)
                continue;

            ComposedDrawCommon common{
                    intent.common.vs, intent.common.ps, intent.common.sp, intent.common.color, intent.common.key};

            const auto pivot = intent.common.pivot;
            const auto rot_z = intent.common.rotation_z;

            const float obj_cos = std::cos(rot_z);
            const float obj_sin = std::sin(rot_z);

            std::variant<ComposedSpriteDesc, ComposedTextDesc, TriangleDrawDesc> special;

            switch (intent.kind)
            {
            case DrawKind::KIND_TRIANGLE: {
                const auto &tri = std::get<TriangleDrawDesc>(intent.special);

                special = TriangleDrawDesc{
                        {Math::transform_pipe_fast(
                                 tri.points[0], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH),
                         Math::transform_pipe_fast(
                                 tri.points[1], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH),
                         Math::transform_pipe_fast(
                                 tri.points[2],
                                 obj_cos,
                                 obj_sin,
                                 pivot,
                                 camera,
                                 cam_cos,
                                 cam_sin,
                                 invHalfW,
                                 invHalfH)}};
                break;
            }

            case DrawKind::KIND_SPRITE: {
                const auto &spr = std::get<SpriteDrawDesc>(intent.special);
                ComposedSpriteDesc composed_spr;
                composed_spr.src_rect = spr.src_rect;

                auto quad = Math::project_rect_local_to_ndc(spr.dst_rect, rot_z, pivot, camera);
                for (int i = 0; i < 4; ++i)
                    composed_spr.dst_rect[i] = quad[i];

                composed_spr.flipX = spr.flipX;
                composed_spr.flipY = spr.flipY;
                special = composed_spr;
                break;
            }

            case DrawKind::KIND_TEXT: {
                const auto &txt = std::get<TextDrawDesc>(intent.special);
                special = ComposedTextDesc{txt.text, common.sp};
                break;
            }

            default:
                continue;
            }

            comp._items.push_back({intent.kind, common, std::move(special)});
        }
    }
} // namespace System::Render
