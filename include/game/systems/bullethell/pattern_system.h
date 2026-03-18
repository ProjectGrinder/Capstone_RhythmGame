#pragma once

#include "game/components.h"

// FIXME: Part of physics, consider separation into its own thread
namespace Game::BulletHell
{
    template<typename T>
    void pattern_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Pattern, Velocity, Rotation, Acceleration, AngularVelocity> &query,
            System::ECS::Query<Battle::BattleState> &query2,
            System::ECS::Query<Battle::PatternContainer> &query3)
    {
        if (query2.begin() == query2.end())
            return;

        if (query2.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
            return;

        if (query3.begin() == query3.end()) return;

        const auto &[pattern_sequences, pattern_steps] = query3.front().get<Battle::PatternContainer>();

        for (auto &[id, comps] : query)
        {
            auto &patt_c = comps.get<Pattern>();

            if (patt_c.sequenceID == 0)
            {
                // Remove Component?
                continue;
            }
            const auto sequenceID = patt_c.sequenceID-1;
            const auto &patt_seq = pattern_sequences[patt_c.sequenceID-1];

            // Check End (Should be -> Remove component?)
            if (patt_c.sequenceIdx >= static_cast<int>(patt_seq.steps.size()))
            {
                continue;
            }

            // Init
            if (patt_c.sequenceIdx == -1)
            {

                patt_c.delay = pattern_steps[patt_seq.steps[0]].delay;
                patt_c.sequenceIdx = patt_c.sequenceIdx + 1;
                continue;
            }

            if (patt_c.delay > 0)
            {
                patt_c.delay -= static_cast<int>(get_delta_time());
            }

            else
            {
                while (patt_c.delay <= 0)
                {
                    const auto patt = pattern_steps[pattern_sequences[sequenceID].steps[patt_c.sequenceIdx]];
                    uint8_t p_idx = 0;
                    const auto op = static_cast<float>(patt.op);

                    if ((patt.mask & 8) > 0)
                    {
                        comps.get<Velocity>().vx = comps.get<Velocity>().vx * op + patt.p[p_idx];
                        p_idx++;
                    }
                    if ((patt.mask & 4) > 0)
                    {
                        comps.get<Rotation>().angleZ = comps.get<Rotation>().angleZ * op + patt.p[p_idx];
                        p_idx++;
                    }
                    if ((patt.mask & 2) > 0)
                    {
                        comps.get<Acceleration>().ax = comps.get<Acceleration>().ax * op + patt.p[p_idx];
                        p_idx++;
                    }
                    if ((patt.mask & 1) > 0)
                    {
                        comps.get<AngularVelocity>().v = comps.get<AngularVelocity>().v * op + patt.p[p_idx];
                    }

                    patt_c.sequenceIdx = patt_c.sequenceIdx + 1;
                    if (patt_c.sequenceIdx >= pattern_sequences[sequenceID].steps.size())
                    {
                        if (pattern_sequences[sequenceID].isLoop)
                            patt_c.sequenceIdx = -1;
                        else
                            patt_c.sequenceID = 0;
                        break;
                    }

                    patt_c.delay = pattern_steps[pattern_sequences[sequenceID].steps[patt_c.sequenceIdx]].delay;
                }
            }
        }
    }
} // namespace Game::BulletHell