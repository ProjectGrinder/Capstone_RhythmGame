#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void HandleRhythm(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Battle::BattleState> &battle_query,
            System::ECS::Query<Rhythm::KeyInput> &input_query,
            [[maybe_unused]] System::ECS::Query<Rhythm::Lane, Rhythm::Timing> &note_query,
            [[maybe_unused]] System::ECS::Query<Rhythm::Lane, Rhythm::Timing, Rhythm::TimingEnd> &hold_note_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        {
            return;
        }

        if (input_query.begin() == input_query.end())
            return;

        // This system should check the input, remove a note if it was hit correctly, and update score/hp/etc. correctly
        // If you need to adjust components or data structures you can ask what structures implement what data to
        // Midfield
        for (auto &[id, comp] : input_query)
        {
            if (comp.get<KeyInput>().input1 == true)
            {
                HandleJudgement(0, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input2 == true)
            {
                HandleJudgement(1, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input3 == true)
            {
                HandleJudgement(2, syscall, battle_query, note_query);
            }
            if (comp.get<KeyInput>().input4 == true)
            {
                HandleJudgement(3, syscall, battle_query, note_query);
            }
        }
    }

    template<typename T>
    void HandleJudgement(
        int lane_num, // assume that first lane is lane 0
        [[maybe_unused]] T &syscall,
        [[maybe_unused]] System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Rhythm::Lane, Rhythm::Timing> &note_query)
    {
        for (auto &[id2, comp2] : note_query)
        {
            if (comp2.get<Lane>().lane == lane_num)
            {
                if (comp2.get<Timing>().timing >= -50 && comp2.get<Timing>().timing <= 50)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.perfect_count += 1;
                    syscall.remove_entity(id2);
                }
                else if (comp2.get<Timing>().timing >= -75 && comp2.get<Timing>().timing <= 75)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.great_count += 1;
                    syscall.remove_entity(id2);
                }
                else if (comp2.get<Timing>().timing >= -100 && comp2.get<Timing>().timing <= 100)
                {
                    battle_query.front().get<Battle::BattleState>().judgement_count.fine_count += 1;
                    syscall.remove_entity(id2);
                }
            }
        }
    }
} // namespace Game::Rhythm
