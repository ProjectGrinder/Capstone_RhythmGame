#pragma once
#include "game/components/render/intent_handle.h"

namespace Game::Render
{
    template<typename T>
    void sprite_intent([[maybe_unused]] T &syscall, System::ECS::Query<Sprite, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::Render::DrawIntent> &intent = System::Render::IntentStorage::get_intent(intent_id);
            const Sprite &sprite = comps.get<Sprite>();

            if (!intent.has_value())
            {
                intent = System::Render::DrawIntent{};
            }

            switch (intent.value().kind)
            {
            case System::Render::DrawKind::KIND_UNKNOWN:
                intent.value().kind = System::Render::DrawKind::KIND_SPRITE;
            case System::Render::DrawKind::KIND_SPRITE:
                break;
            default:
                return;
            }

            if (!std::holds_alternative<System::Render::SpriteDrawDesc>(intent.value().special))
            {
                intent.value().special = System::Render::SpriteDrawDesc{};
            }

            auto &common = intent.value().common;
            common.layer = sprite.layer;
            common.order = sprite.order;

            auto &[texture, src_rect, dst_rect, flipX, flipY] =
                    std::get<System::Render::SpriteDrawDesc>(intent.value().special);
            texture = sprite.texture;
            src_rect = System::Render::Rect{sprite.src_rect.u0, sprite.src_rect.v0, sprite.src_rect.u1, sprite.src_rect.v1};
            dst_rect = System::Render::Rect{sprite.dst_rect.u0, sprite.dst_rect.v0, sprite.dst_rect.u1, sprite.dst_rect.v1};
            flipX = sprite.flipX;
            flipY = sprite.flipY;
        }
    }
} // namespace Game::Render
