#pragma once
#include "game/components.h"
#include "pattern_container.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    template<typename T>
    void PatternSystem(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Pattern, Velocity, Rotation, Acceleration, AngularVelocity> &query,
            System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        constexpr auto frame_time = 1;

        for (auto &[id, comps]: query)
        {
            auto &patt_c = comps.get<Pattern>();
            const auto &patt_seq = pattern_sequences[patt_c.sequenceID];

            // Check End (Choose be pop out component?)
            if (patt_c.sequenceIdx >= patt_seq.stepCount) continue;

            // Init
            if (patt_c.sequenceIdx == -1)
            {
                patt_c.delay = patt_seq.step[0].delay;
                patt_c.sequenceIdx = patt_c.sequenceIdx + 1;
                continue;
            }

            if (patt_c.delay > 0)
            {
                patt_c.delay -= frame_time;
            }

            else
            {
                const auto patt = pattern_sequences[patt_c.sequenceID].step[patt_c.sequenceIdx];
                patt_c.delay = patt.delay;
                uint8_t p_idx = 0;

                if (patt.mask & 8 > 0)
                {
                    comps.get<Velocity>().vx = comps.get<Velocity>().vx * patt.op + patt.p[p_idx];
                    p_idx++;
                }
                if (patt.mask & 4 > 0)
                {
                    comps.get<Rotation>().angleZ = comps.get<Rotation>().angleZ * patt.op + patt.p[p_idx];
                    p_idx++;
                }
                if (patt.mask & 2 > 0)
                {
                    comps.get<Acceleration>().ax = comps.get<Acceleration>().ax * patt.op + patt.p[p_idx];
                    p_idx++;
                }
                if (patt.mask & 1 > 0)
                {
                    comps.get<AngularVelocity>().v = comps.get<AngularVelocity>().v * patt.op + patt.p[p_idx];
                }

                patt_c.sequenceIdx = patt_c.sequenceIdx + 1;
                if (patt_c.sequenceIdx >= pattern_sequences[patt_c.sequenceID].stepCount)
                {
                    if (pattern_sequences[patt_c.sequenceID].isLoop)
                    {
                        patt_c.sequenceIdx = -1;
                    }
                }
            }
        }
    }
}