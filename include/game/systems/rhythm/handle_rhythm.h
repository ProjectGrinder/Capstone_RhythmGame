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
        // TODO: Pooh part please implement
    }
} // namespace Game::Rhythm
