#pragma once
#include "game/utils/physics_util.h"

namespace Game::Physics
{
    template<typename T>
    void ShakingSystem(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Shaker, Position, Rotation> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        constexpr auto frame_time = 1; // 1ms
        for (auto &[id, comps]: query)
        {
            auto &shaker_c = comps.get<Shaker>();
            auto &pos = comps.get<Position>();
            const auto &rot = comps.get<Rotation>();

            if (shaker_c.delay > 0)
            {
                shaker_c.delay -= frame_time;
            }
            else
            {
                if (shaker_c.shakingTime > 0)
                {
                    shaker_c.shakingTime -= frame_time;
                    if (get_distance(shaker_c.last_position, pos) >
                        (shaker_c.magnitudeX > shaker_c.magnitudeY ? shaker_c.magnitudeX : shaker_c.magnitudeY))
                    {
                        shaker_c.last_position = pos;
                    }

                    if (shaker_c.last_position.x == 0 && shaker_c.last_position.y == 0)
                    {
                        shaker_c.last_position = pos;
                    }

                    pos.x = pos.x + shaker_c.magnitudeX * cos(rot.angleX) * rand_float(-1, 1);
                    pos.y = pos.y + shaker_c.magnitudeY * sin(rot.angleY) * rand_float(-1, 1);
                }
            }
        }
    }
} // namespace Game::Physics
