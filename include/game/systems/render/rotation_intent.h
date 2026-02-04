#pragma once

namespace Game::Render
{
    template<typename T>
    void rotation_intent([[maybe_unused]] T &syscall, System::ECS::Query<Rotation, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::DrawIntent> &intent = System::RenderStorage::get_intent(intent_id);
            const Rotation &rotation = comps.get<Rotation>();

            if (!intent.has_value())
            {
                intent = System::DrawIntent{};
            }

            auto &common = intent.value().common;
            common.rotation_z = rotation.angleZ;
        }
    }
} // namespace Game::Render
