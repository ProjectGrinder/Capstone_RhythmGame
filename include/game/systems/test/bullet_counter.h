#pragma once

extern "C" long double get_delta_time();
namespace Game::Test
{
    struct BulletCounter
    {
    };

    template<typename T>
    void bullet_counter([[maybe_unused]] T &syscall, System::ECS::Query<Render::Text, BulletCounter> &query, System::ECS::Query<BulletHell::Bullet> &bullet_query)
    {
        if (query.begin() == query.end())
            return;

        auto &text = query.front().get<Render::Text>();

        text.text = "Bullet Count : " + std::to_string(bullet_query.size());

    }

} // namespace Game::Test
