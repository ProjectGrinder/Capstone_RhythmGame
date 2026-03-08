#pragma once

namespace Game::Render
{
    template <typename T>
    void triangle_intent([[maybe_unused]] T &syscall, System::ECS::Query<Triangle, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::Render::DrawIntent> &intent = System::Render::IntentStorage::get_intent(intent_id);
            const Triangle &triangle = comps.get<Triangle>();

            if (!intent.has_value())
            {
                intent = System::Render::DrawIntent{};
            }

            if (!intent.has_value())
            {
                intent = System::Render::DrawIntent{};
            }

            switch (intent.value().kind)
            {
            case System::Render::DrawKind::KIND_UNKNOWN:
                intent.value().kind = System::Render::DrawKind::KIND_TRIANGLE;
            case System::Render::DrawKind::KIND_TRIANGLE:
                break;
            default:
                return;
            }

            if (!std::holds_alternative<System::Render::TriangleDrawDesc>(intent.value().special))
            {
                intent.value().special = System::Render::TriangleDrawDesc{};
            }

            auto &common = intent.value().common;
            common.layer = triangle.layer;
            common.order = triangle.order;

            auto &special = std::get<System::Render::TriangleDrawDesc>(intent.value().special);
            special.points[0] = triangle.points[0];
            special.points[1] = triangle.points[1];
            special.points[2] = triangle.points[2];
        }
    }
}