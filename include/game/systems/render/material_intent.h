#pragma once
#include "game/components/render/intent_handle.h"

namespace Game::Render
{
    template<typename T>
    void MaterialIntent([[maybe_unused]] T &syscall, System::ECS::Query<Material, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::DrawIntent> &intent = System::RenderStorage::get_intent(intent_id);
            const Material &material = comps.get<Material>();

            if (!intent.has_value())
            {
                intent = System::DrawIntent{};
            }
            auto &common = intent.value().common;
            common.vert_shader = material.vert_shader;
            common.pixel_shader = material.pixel_shader;
            common.visible = material.visible;
            common.render_prior = material.render_prior;
            common.color = System::Color(material.color.r, material.color.g, material.color.b, material.color.a);
        }
    }
} // namespace Game::Render
