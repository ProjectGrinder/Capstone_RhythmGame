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

            auto &[u0, v0, u1, v1, u2, v2] = std::get<System::Render::TriangleDrawDesc>(intent.value().special);
            u0 = triangle.points[0].x;
            v0 = triangle.points[0].y;
            u1 = triangle.points[1].x;
            v1 = triangle.points[1].y;
            u2 = triangle.points[2].x;
            v2 = triangle.points[2].y;
        }
    }
}