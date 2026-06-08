#pragma once

namespace Game::Test
{
    struct BulletCounter {};
    struct GrazeText{};
    struct LifeText{};
}
namespace Game::Battle
{
    // use these for persistent UI that stays for the entire level
    template<typename T>
    void handle_text_ui(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState, BulletHellState, RhythmState> &state_query,
        System::ECS::Query<UIComponent, Render::Text> &ui_query)
    {
        if (state_query.begin() == state_query.end())
            return;

        for (auto &[id, comp] : ui_query)
        {
            if (comp.get<UIComponent>().type == Battle::AccuracyText)
            {
                int accuracy = static_cast<int>(state_query.front().get<RhythmState>().accuracy);
                if (accuracy > 10000)
                    accuracy = 10000;

                std::string acc_text = std::to_string(accuracy);
                std::string final_text;

                if (acc_text.empty())
                    return;

                if (acc_text.length() == 1)
                {
                    final_text = "0.0" + acc_text.append("%");
                }
                else if (acc_text.length() == 2)
                {
                    final_text = "0." + acc_text.append("%");
                }
                else
                {
                    std::string number = acc_text.substr(0, acc_text.length() - 2);
                    std::string decimal = acc_text.substr(acc_text.length() - 2);
                    final_text = number.append("." + decimal.append("%"));
                }
                comp.get<Render::Text>().text = final_text;
                state_query.front().get<Battle::RhythmState>().accuracy_text = final_text;
                if (accuracy == 10000)
                {
                    comp.get<Render::Text>().color = Math::Color{0.3f, 1, 0.3f};
                }
            }

            if (comp.get<UIComponent>().type == Battle::GrazeText)
            {
                const int graze = (state_query.front().get<BulletHellState>().graze);

                comp.get<Render::Text>().text = std::to_string(graze);
                if (graze >= 20)
                {
                    comp.get<Render::Text>().color = Math::Color{0.3f, 1, 0.3f};
                }
            }

            if (comp.get<UIComponent>().type ==LevelDiff)
            {
                const auto level = state_query.front().get<LevelData>().difficulty;
                switch (level.difficulty)
                {
                    case LIGHT:
                    comp.get<Render::Text>().text = "LIGHT " + std::to_string(level.level);
                    comp.get<Render::Text>().color = Math::Color{0, 0.5f, 1};
                    break;

                    case SPARK:
                    comp.get<Render::Text>().text = "SPARK " + std::to_string(level.level);
                    comp.get<Render::Text>().color = Math::Color{1, 0.5f, 0};
                    break;

                    case BLAZE:
                    comp.get<Render::Text>().text = "BLAZE " + std::to_string(level.level);
                    comp.get<Render::Text>().color = Math::Color{1, 0, 0};
                    break;

                    case ASTRA:
                    comp.get<Render::Text>().text = "ASTRA " + std::to_string(level.level);
                    comp.get<Render::Text>().color = Math::Color{1, 0, 1};
                    break;

                    default:
                    comp.get<Render::Text>().text = std::to_string(level.level);
                }
            }
        }
    }

    template<typename T>
    void handle_sprite_ui(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState, BulletHellState, RhythmState> &state_query,
        System::ECS::Query<UIComponent, Render::Sprite> &ui_query)
    {
        if (state_query.begin() == state_query.end())
            return;

        for (auto &[id, comp] : ui_query)
        {
            if (comp.get<UIComponent>().type == Battle::AcceptBar)
            {
                auto accept = state_query.front().get<BattleState>().current_accept;
                const auto max_accept = state_query.front().get<BattleState>().max_accept_gauge;
                if (accept > max_accept)
                    accept = max_accept;

                const float accept_percent = static_cast<float>(accept)/static_cast<float>(max_accept);
                constexpr float end_bar_pos = 365.f;
                const float remain_dest_x = -end_bar_pos + end_bar_pos*2*accept_percent;

                comp.get<Render::Sprite>().pos[1].x = remain_dest_x;
                comp.get<Render::Sprite>().pos[2].x = remain_dest_x;

                if (accept_percent >= 0.7f)
                {
                    comp.get<Render::Sprite>().color = Math::Color{0, 0.8f, 0.9f};
                }
                else
                {
                    comp.get<Render::Sprite>().color = Math::Color{0, 0.5f, 1};
                }
            }
            if (comp.get<UIComponent>().type == Battle::HpBar)
            {
                const auto hp = state_query.front().get<BattleState>().hp;
                const auto max_hp = state_query.front().get<BattleState>().max_hp;

                const float hp_percent = static_cast<float>(hp)/static_cast<float>(max_hp);
                constexpr float end_bar_pos = 150.f;
                const float remain_dest_x = -end_bar_pos + end_bar_pos*2*hp_percent;

                comp.get<Render::Sprite>().pos[1].x = remain_dest_x;
                comp.get<Render::Sprite>().pos[2].x = remain_dest_x;
            }
        }
    }

} // namespace Game::Test
