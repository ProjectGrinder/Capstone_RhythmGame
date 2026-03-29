#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;

    inline void handle_speed_change(
        const int render_offset,
        const int clock_time,
        System::ECS::Query<Position, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        const NoteField &field)
    {
        const auto rhythm_state = rhythm_query.front().get<Battle::RhythmState>();
        // If slow down, consider unrendered notes being in the field
        if (rhythm_state.current_speed < rhythm_state.base_speed)
        {
            for (auto &[id, comp]: note_query)
            {
                if (clock_time - comp.get<Timing>().timing < render_offset)
                {
                    if (comp.get<NoteStatus>().state == 1)
                    {
                        // slow down will pull notes down
                        const auto old_y = comp.get<Position>().y;
                        comp.get<Position>().y -= old_y * (1 - (rhythm_state.current_speed / rhythm_state.base_speed));
                    }
                    else if (comp.get<NoteStatus>().state == -1)
                    {
                        const auto time_diff = clock_time - comp.get<Timing>().timing;
                        comp.get<Position>().y = field.judge_level + (field.spawn_level - field.judge_level) *
                            (static_cast<float>(time_diff) / static_cast<float>(render_offset));
                        comp.get<NoteStatus>().state = 1;
                    }
                }
            }
        }
        // If speed up, consider rendered notes being off the field
        else
        {
            for (auto &[id, comp]: note_query)
            {
                if (comp.get<NoteStatus>().state == 1)
                {
                    if (clock_time - comp.get<Timing>().timing <= render_offset)
                    {
                        // speed up will push notes up
                        const auto old_y = comp.get<Position>().y;
                        comp.get<Position>().y += old_y * (1 - (rhythm_state.base_speed / rhythm_state.current_speed));
                    }
                    else
                    {
                        comp.get<Position>().y = field.spawn_level;
                        comp.get<NoteStatus>().state = -1;
                    }
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

        constexpr auto frame_time = 1;
        const auto field = field_query.front().get<NoteField>();
        const auto current_speed = rhythm_query.front().get<Battle::RhythmState>().current_speed;
        const int render_offset = static_cast<int>((field.spawn_level - field.judge_level) / current_speed); //make sure to test and adjust
        const auto clock_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;

        // If speed change is detected, change note positions
        if (rhythm_query.front().get<Battle::RhythmState>().speed_change)
        {
            handle_speed_change(render_offset, clock_time, note_query, rhythm_query, field);
            rhythm_query.front().get<Battle::RhythmState>().speed_change = false;
            return;
        }
        // Notes always move in constant speed
        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state == 1)
            {
                // modify to what seems fit
                comp.get<Position>().y -= current_speed * frame_time;
            }
            else if (comp.get<NoteStatus>().state == -1)
            {
                //check if note is in render timing
                if (comp.get<Timing>().timing - clock_time <= render_offset)
                {
                    comp.get<NoteStatus>().state = 1;
                }
            }
        }
    }
}