#pragma once
#include <system.h>
#include "game/components/render/material.h"
#include "game/components/render/transform.h"
#include "game/components/render/triangle.h"
#include "system/asset_manager.h"
#include "system/intent_storage.h"

namespace Game::Render
{
    namespace Helpers
    {
        inline void
        fill_common(System::Render::DrawIntent &intent, const Material &mat, const Transform &tra, uint8_t layer)
        {
            auto &c = intent.common;

            c.vs = mat.vs;
            c.ps = mat.ps;
            c.sp = nullptr;
            c.color = System::Render::Color{mat.color.r, mat.color.g, mat.color.b, mat.color.a};
            c.pivot = tra.position;
            c.rotation_z = tra.angleZ;

            c.info.layer = layer;
            c.info.vs_id = ASSET_INDEX(mat.vs->id);
            c.info.ps_id = ASSET_INDEX(mat.ps->id);
            c.info.sp_id = (uint16_t) -1;
            c.info.padding = 0;
        }
    } // namespace Helpers

    template<typename T>
    void draw_triangle([[maybe_unused]] T &syscall, System::ECS::Query<Triangle, Material, Transform> &query)
    {
        for (auto &[id, comps]: query)
        {
            const auto &material = comps.get<Material>();
            if (!material.visible)
                continue;

            const auto &tri = comps.get<Triangle>();
            const auto &tra = comps.get<Transform>();

            auto &intent = System::Render::IntentStorage::allocate_packed();
            intent.kind = System::Render::DrawKind::KIND_TRIANGLE;

            Helpers::fill_common(intent, material, tra, (uint8_t) tri.layer);

            intent.special = System::Render::TriangleDrawDesc{{tri.points[0], tri.points[1], tri.points[2]}};
        }
    }
} // namespace Game::Render
