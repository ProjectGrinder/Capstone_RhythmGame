#pragma once

namespace Game::Render
{
    template<typename T>
    void TextIntent([[maybe_unused]] T &syscall, System::ECS::Query<Text, IntentHandle> &query)
    {
        for (auto &[id, comps] : query)
        {
            const size_t &intent_id = comps.get<IntentHandle>().handle_id;
            std::optional<System::DrawIntent> &intent = System::RenderStorage::get_intent(intent_id);
            const Text &text_component = comps.get<Text>();

            if (!intent.has_value())
            {
                intent = System::DrawIntent{};
            }

            switch (intent.value().kind)
            {
            case System::DrawKind::KIND_UNKNOWN:
                intent.value().kind = System::DrawKind::KIND_TEXT;
            case System::DrawKind::KIND_TEXT:
                break;
            default:
                return;
            }

            if (!std::holds_alternative<System::TextDrawDesc>(intent.value().special))
            {
                intent.value().special = System::TextDrawDesc{};
            }

            auto &[text, font_name, font_size] = std::get<System::TextDrawDesc>(intent.value().special);
            text = std::string_view(text_component.text);
            font_name = text_component.font_name;
            font_size = text_component.font_size;
        }
    }
}