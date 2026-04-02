#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;

    inline void handle_speed_change(
        const float render_offset,
        const int clock_time,
        System::ECS::Query<Position, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        const NoteField &field)
    {
        const auto height = field.judge_level - field.spawn_level;

        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state != 0)
            {
                const auto note_diff = static_cast<float>(clock_time - comp.get<Timing>().timing);
                if (note_diff < render_offset)
                {
                    comp.get<Position>().y = field.judge_level - height * (static_cast<float>(note_diff) / render_offset);
                    comp.get<NoteStatus>().state = 1;
                }
                else
                {
                    comp.get<Position>().y = field.spawn_level;
                    comp.get<NoteStatus>().state = -1;
                }
            }
        }
    }

    template<typename T>
    void update_notes(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Position, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<NoteField> &field_query)
    {
        if (note_query.begin() == note_query.end())
            return;

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (rhythm_query.begin() == rhythm_query.end())
            return;

        const auto field = field_query.front().get<NoteField>();
        const auto current_speed = rhythm_query.front().get<Battle::RhythmState>().current_speed;
        const float render_offset = (field.spawn_level - field.judge_level) / current_speed; //make sure to test and adjust
        const auto clock_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;

        // If speed change is detected, change note positions
        if (rhythm_query.front().get<Battle::RhythmState>().speed_change)
        {
            handle_speed_change(render_offset, clock_time, note_query, field);
            rhythm_query.front().get<Battle::RhythmState>().speed_change = false;
            return;
        }
        // Notes always move in constant speed
        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state == 1)
            {
                constexpr auto frame_time = 1;
                // modify to what seems fit
                comp.get<Position>().y += current_speed * frame_time;
            }
            else if (comp.get<NoteStatus>().state == -1)
            {
                //check if note is in render timing
                if (static_cast<float>(comp.get<Timing>().timing - clock_time) <= render_offset)
                {
                    comp.get<NoteStatus>().state = 1;
                }
            }
        }
    }
}