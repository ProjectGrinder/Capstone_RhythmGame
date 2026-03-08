#pragma once

namespace Game::Render
{
    template<typename T>
    void transform_intent([[maybe_unused]] T &syscall, System::ECS::Query<Transform, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::Render::DrawIntent> &intent = System::Render::IntentStorage::get_intent(intent_id);
            const Transform &transform = comps.get<Transform>();

            if (!intent.has_value())
            {
                intent = System::Render::DrawIntent{};
            }

            auto &common = intent.value().common;
            common.pivot = transform.position;
            common.rotation_z = transform.angleZ;
        }
    }
} // namespace Game::Render
