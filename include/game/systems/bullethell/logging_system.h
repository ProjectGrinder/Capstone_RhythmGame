#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void LoggingSystem([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState, Battle::BulletHellState, Battle::BulletLoader> &query, System::ECS::Query<Player, Position> &query2, System::ECS::Query<Bullet, Delay, Position, Rotation, Physics::Scale, Velocity> &query3)
    {
        if (query.begin() == query.end())
            return;

        if (query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        // constexpr auto frame_time = 0.1;

        const auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        LOG_INFO("Frame %d", bullet_loader.current_frame);

        LOG_INFO("Hp : %d, iFrame : %d, Bullet Pointer : %d", query.front().get<Battle::BattleState>().hp, query.front().get<Battle::BulletHellState>().iframe_time, bullet_loader.pointer);
        const auto player_pos = query2.front().get<Position>();
        LOG_INFO("Player Pos : (%d,%d)", static_cast<int>(player_pos.x), static_cast<int>(player_pos.y));

        LOG_INFO("%d bullets on screen : ", query3.size());
        for (auto &[id, comps] : query3)
        {
            LOG_INFO("Bullet of ID<%d> : ", id);
            if (!comps.get<Bullet>().is_damageable)
            {
                LOG_INFO("is delay for %d frames", comps.get<Delay>().delay);
            }
            else
            LOG_INFO(
                "at pos<%d,%d> of size<%d,%d> at speed<%d>, at rot<%d>", round(comps.get<Position>().x), round(comps.get<Position>().y), round(comps.get<Physics::Scale>().scaleX), round(comps.get<Physics::Scale>().scaleY), round(comps.get<Velocity>().vx), round(comps.get<Rotation>().angleZ)
            );
        }
        LOG_INFO("----------------------------------");
    }
}