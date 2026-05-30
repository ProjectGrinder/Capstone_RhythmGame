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
                // TODO: make this into 2-place decimal
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
            if (comp.get<UIComponent>().type == Battle::HpBar)
            {
                const auto hp = state_query.front().get<BattleState>().hp;
                const auto max_hp = state_query.front().get<BattleState>().max_hp;
                const float hp_remain_dest_x = -100.f + 200.f * static_cast<float>(hp)/static_cast<float>(max_hp);

                comp.get<Render::Sprite>().pos[1] = {hp_remain_dest_x,10,0};
                comp.get<Render::Sprite>().pos[2] = {hp_remain_dest_x,-10,0};
            }
            // TODO: add acceptance gauge
        }
    }

} // namespace Game::Test
