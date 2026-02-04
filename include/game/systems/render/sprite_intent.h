#pragma once
#include "game/components/render/intent_handle.h"

namespace Game::Render
{
    template<typename T>
    void SpriteIntent([[maybe_unused]] T &syscall, System::ECS::Query<Sprite, IntentHandle> &query)
    {
        for (auto &[id, comps]: query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::DrawIntent> &intent = System::RenderStorage::get_intent(intent_id);
            const Sprite &sprite = comps.get<Sprite>();

            if (!intent.has_value())
            {
                intent = System::DrawIntent{};
            }

            switch (intent.value().kind)
            {
            case System::DrawKind::KIND_UNKNOWN:
                intent.value().kind = System::DrawKind::KIND_SPRITE;
            case System::DrawKind::KIND_SPRITE:
                break;
            default:
                return;
            }

            if (!std::holds_alternative<System::SpriteDrawDesc>(intent.value().special))
            {
                intent.value().special = System::SpriteDrawDesc{};
            }

            auto &common = intent.value().common;
            common.layer = sprite.layer;
            common.order = sprite.order;

            auto &[texture, src_rect, dst_rect, flipX, flipY] =
                    std::get<System::SpriteDrawDesc>(intent.value().special);
            texture = sprite.texture;
            src_rect = System::Rect{sprite.src_rect.u0, sprite.src_rect.v0, sprite.src_rect.u1, sprite.src_rect.v1};
            dst_rect = System::Rect{sprite.dst_rect.u0, sprite.dst_rect.v0, sprite.dst_rect.u1, sprite.dst_rect.v1};
            flipX = sprite.flipX;
            flipY = sprite.flipY;
        }
    }
} // namespace Game::Render
