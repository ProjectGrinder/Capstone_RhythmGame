#pragma once

extern "C" long double get_delta_time();
namespace Game::Test
{
    struct BulletCounter
    {
    };

    struct LifeText
    {
    };

    struct GrazeText{};
    struct AccuracyText{};

    template<typename T>
    void stat_text_render([[maybe_unused]] T &syscall,
        // System::ECS::Query<Render::Text, BulletCounter> &query,
        // System::ECS::Query<Render::Text, LifeText> &query2,
        // System::ECS::Query<Render::Text, GrazeText> &query3,
        System::ECS::Query<Battle::UIComponent, Render::Sprite> &ui_query,
        System::ECS::Query<AccuracyText, Render::Text> &acc_query,
        //System::ECS::Query<BulletHell::Bullet> &bullet_query,
        System::ECS::Query<Battle::BattleState, Battle::BulletHellState, Battle::RhythmState> &state_query)
    {
        // if (query.begin() == query.end())
        //     return;

        auto hp = state_query.front().get<Battle::BattleState>().hp;
        const auto max_hp = state_query.front().get<Battle::BattleState>().max_hp;
        if (hp < 0)
        {
            hp = 0;
            state_query.front().get<Battle::BattleState>().hp = 0;
        }
        else if (hp > max_hp)
        {
            hp = max_hp;
            state_query.front().get<Battle::BattleState>().hp = max_hp;
        }

        auto current_accept = state_query.front().get<Battle::BattleState>().current_accept;
        const auto max_accept = state_query.front().get<Battle::BattleState>().max_accept_gauge;
        if (current_accept < 0)
        {
            current_accept = 0;
            state_query.front().get<Battle::BattleState>().current_accept = 0;
        }
        else if (current_accept > max_accept)
        {
            current_accept = max_accept;
            state_query.front().get<Battle::BattleState>().current_accept = max_accept;
        }


        // query.front().get<Render::Text>().text = "Bullet Count : " + std::to_string(bullet_query.size());

        // query2.front().get<Render::Text>().text = "Life : " + std::to_string(hp) + "/" + std::to_string(max_hp);

        const float hp_remain_dest_x = -100.f + 200.f * static_cast<float>(hp)/static_cast<float>(max_hp);

        for (auto &[id, comps] : ui_query)
        {
            if (comps.get<Battle::UIComponent>().type == Battle::HpBar)
            {
                comps.get<Render::Sprite>().pos[1] = {hp_remain_dest_x,10,0};
                comps.get<Render::Sprite>().pos[2] = {hp_remain_dest_x,-10,0};
            }
        }

        int accuracy = static_cast<int>(state_query.front().get<Battle::RhythmState>().accuracy);
        acc_query.front().get<Render::Text>().text = std::to_string(accuracy) + "%";

        // query3.front().get<Render::Text>().text = "Graze : " + std::to_string(state_query.front().get<Battle::BulletHellState>().graze);

    }

} // namespace Game::Test
