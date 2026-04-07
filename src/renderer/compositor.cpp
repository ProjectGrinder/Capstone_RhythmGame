#include "system/compositor.h"
#include <cmath>
#include "maths/types.h"
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

    Compositor::Compositor()
    {
        _sort_entries.reserve(PRESIZE);
        _sort_temp.reserve(PRESIZE);
        _items.reserve(PRESIZE);
        _reserve_size = PRESIZE;
    }

    void Compositor::radix_sort()
    {
        _total_count = _sort_entries.size();
        if (_total_count == 0)
            return;

        SortEntry *idx_src = _sort_entries.data();
        SortEntry *idx_dst = _sort_temp.data();

        for (uint32_t p = 0; p < PASSES; ++p)
        {
            _counts.fill(0);
            int shift = p * 8;

            for (size_t i = 0; i < _total_count; ++i)
                ++_counts[(idx_src[i].key >> shift) & 0xFF];

            _offsets[0] = 0;
            for (int i = 1; i < 256; ++i)
                _offsets[i] = _offsets[i - 1] + _counts[i - 1];

            for (size_t i = 0; i < _total_count; ++i)
            {
                uint8_t bucket = (idx_src[i].key >> shift) & 0xFF;
                idx_dst[_offsets[bucket]++] = idx_src[i];
            }
            std::swap(idx_src, idx_dst);
        }

        if constexpr (PASSES % 2 != 0)
        {
            std::memcpy(_sort_entries.data(), _sort_temp.data(), _total_count * sizeof(SortEntry));
        }
    }

    void Compositor::resize(size_t size)
    {
        _sort_temp.reserve(size);
        _sort_entries.reserve(size);
        _items.reserve(size);
        _reserve_size = size;
    }

    void Compositor::compose(const std::vector<DrawIntent> &intents, const Camera &camera)
    {
        auto &comp = instance();
        const float cam_cos = std::cos(-camera.rotation);
        const float cam_sin = std::sin(-camera.rotation);
        const float invHalfW = (camera.scaleX != 0.0f) ? (2.0f / camera.scaleX) : 0.0f;
        const float invHalfH = (camera.scaleY != 0.0f) ? (2.0f / camera.scaleY) : 0.0f;

        comp.reset();
        if (comp.get_capacity() < intents.size())
        {
            comp.resize(intents.size());
        }

        for (uint32_t i = 0; i < intents.size(); ++i)
        {
            if (intents[i].common.visible)
            {
                comp.add_sort_entry(SortEntry{intents[i].common.key, i});
            }
        }

        comp.radix_sort();
        RenderItem *current_batch = nullptr;

        auto idx_src = comp.get_sort_data();

        for (size_t i = 0; i < comp.get_total_count(); ++i)
        {
            const auto &intent = intents[idx_src[i].intent_index];
            size_t current_params_size = (intent.kind == DrawKind::KIND_SPRITE) ? sizeof(Math::UV) + sizeof(Math::Color) : 0;
            if (current_batch == nullptr || current_batch->sort_key.as_key != intent.common.key ||
                current_batch->params_size != current_params_size)
            {
                RenderItem new_batch{};
                new_batch.sort_key.as_key = intent.common.key;
                new_batch.vs = const_cast<AssetsRecord *>(intent.common.vs);
                new_batch.ps = const_cast<AssetsRecord *>(intent.common.ps);
                new_batch.sp = const_cast<AssetsRecord *>(intent.common.sp);
                new_batch.params_size = current_params_size;

                new_batch.points.reserve(100);
                new_batch.indices.reserve(150);

                comp._items.push_back(std::move(new_batch));
                current_batch = &comp._items.back();
            }

            const auto pivot = intent.common.pivot;
            const auto rot_z = intent.common.rotation_z;
            const float obj_cos = std::cos(rot_z);
            const float obj_sin = std::sin(rot_z);

            uint32_t current_vertex_offset = (uint32_t) current_batch->points.size();

            switch (intent.kind)
            {
            case DrawKind::KIND_TRIANGLE: {
                const auto &tri = std::get<TriangleDrawDesc>(intent.special);

                current_batch->points.push_back(Math::transform_pipe_fast(
                        tri.points[0], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));
                current_batch->points.push_back(Math::transform_pipe_fast(
                        tri.points[1], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));
                current_batch->points.push_back(Math::transform_pipe_fast(
                        tri.points[2], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));

                current_batch->indices.push_back(current_vertex_offset + 0);
                current_batch->indices.push_back(current_vertex_offset + 1);
                current_batch->indices.push_back(current_vertex_offset + 2);
                break;
            }

            case DrawKind::KIND_SPRITE: {
                const auto &spr = std::get<SpriteDrawDesc>(intent.special);

                current_batch->points.push_back(Math::transform_pipe_fast(
                        spr.points[0], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));
                current_batch->points.push_back(Math::transform_pipe_fast(
                        spr.points[1], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));
                current_batch->points.push_back(Math::transform_pipe_fast(
                        spr.points[2], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));
                current_batch->points.push_back(Math::transform_pipe_fast(
                        spr.points[3], obj_cos, obj_sin, pivot, camera, cam_cos, cam_sin, invHalfW, invHalfH));

                current_batch->indices.push_back(current_vertex_offset + 0);
                current_batch->indices.push_back(current_vertex_offset + 1);
                current_batch->indices.push_back(current_vertex_offset + 2);
                current_batch->indices.push_back(current_vertex_offset + 0);
                current_batch->indices.push_back(current_vertex_offset + 2);
                current_batch->indices.push_back(current_vertex_offset + 3);

                const float u0 = spr.flipX ? spr.u1 : spr.u0;
                const float u1 = spr.flipX ? spr.u0 : spr.u1;
                const float v0 = spr.flipY ? spr.v1 : spr.v0;
                const float v1 = spr.flipY ? spr.v0 : spr.v1;

                struct SpriteParam
                {
                    Math::UV uv;
                    Math::Color color;
                };

                struct SpriteParam sprite_params[4] = {
                    {{u0, v0}, spr.color},
                    {{u1, v0}, spr.color},
                    {{u1, v1}, spr.color},
                    {{u0, v1}, spr.color}
                };

                uint8_t *sprite_params_ptr = reinterpret_cast<uint8_t *>(sprite_params);
                current_batch->param_data.insert(current_batch->param_data.end(), sprite_params_ptr, sprite_params_ptr + sizeof(sprite_params));
                break;
            }
            default:
                break;
            }
        }
    }
} // namespace System::Render
