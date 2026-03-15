#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    template <typename T>
    void logging_system([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState, Battle::BulletHellState, Battle::BulletLoader> &query, System::ECS::Query<Player, Position> &query2, System::ECS::Query<Bullet, Delay, Position, Rotation, Physics::Scale, Velocity> &query3)
    {
        if (query.begin() == query.end())
            return;

        auto &battle_state = query.front().get<Battle::BattleState>();

        if (battle_state.current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        if (battle_state.clock_time-battle_state.last_clocktime <= 250000)
        {
            return;
        }

        battle_state.last_clocktime = battle_state.clock_time;
        LOG_INFO("----------------------------------");

        // const auto &bullet_loader = query.front().get<Battle::BulletLoader>();
        LOG_INFO("Time : %d.%d%d", battle_state.clock_time/1000000, battle_state.clock_time/10000 - (battle_state.clock_time/1000000)*100);

        LOG_INFO("Hp : %d, iFrame : %d, Bullet Pointer : %d", battle_state.hp, query.front().get<Battle::BulletHellState>().iframe_time, 0);//bullet_loader.pointer);
        const auto player_pos = query2.front().get<Position>();
        LOG_INFO("Player Pos : (%d,%d)", static_cast<int>(player_pos.x), static_cast<int>(player_pos.y));

        LOG_INFO("%d bullets on screen : ", query3.size());
        for (auto &[id, comps] : query3)
        {
            if (!comps.get<Bullet>().is_damageable)
            {
                LOG_INFO("Bullet of ID<%d> is delay for %d frames",id , comps.get<Delay>().delay);
            }
            else
            LOG_INFO(
                "Bullet of ID<%d> at pos<%d,%d> of size<%d,%d> at speed<%d>, at rot<%d>", id, static_cast<int>(round(comps.get<Position>().x)), static_cast<int>(round(comps.get<Position>().y)), static_cast<int>(round(comps.get<Physics::Scale>().scaleX)), static_cast<int>(round(comps.get<Physics::Scale>().scaleY)), static_cast<int>(round(comps.get<Velocity>().vx)), static_cast<int>(round(comps.get<Rotation>().angleZ))
            );
        }
        LOG_INFO("----------------------------------");
    }
}