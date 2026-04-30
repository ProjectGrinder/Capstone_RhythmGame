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

    template<typename T>
    void stat_text_render([[maybe_unused]] T &syscall,
        System::ECS::Query<Render::Text, BulletCounter> &query,
        System::ECS::Query<Render::Text, LifeText> &query2,
        System::ECS::Query<Render::Text, GrazeText> &query3,
        System::ECS::Query<Battle::HpBar, Render::Sprite> &hpBar,
        System::ECS::Query<BulletHell::Bullet> &bullet_query,
        System::ECS::Query<Battle::BattleState, Battle::BulletHellState> &state_query)
    {
        if (query.begin() == query.end())
            return;

        const auto hp = state_query.front().get<Battle::BattleState>().hp;
        const auto max_hp = state_query.front().get<Battle::BattleState>().max_hp;

        query.front().get<Render::Text>().text = "Bullet Count : " + std::to_string(bullet_query.size());

        query2.front().get<Render::Text>().text = "Life : " + std::to_string(hp) + "/" + std::to_string(max_hp);

        const float hp_remain_dest_x = -50.f + 100.f * static_cast<float>(hp)/static_cast<float>(max_hp);

        hpBar.front().get<Render::Sprite>().pos[1] = {hp_remain_dest_x,10,0};

        hpBar.front().get<Render::Sprite>().pos[2] = {hp_remain_dest_x,-10,0};

        query3.front().get<Render::Text>().text = "Graze : " + std::to_string(state_query.front().get<Battle::BulletHellState>().graze);

    }

} // namespace Game::Test
