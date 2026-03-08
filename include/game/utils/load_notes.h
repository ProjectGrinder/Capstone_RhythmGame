#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void
    create_note_entity(T &syscall, Battle::RhythmState &rhythm_state, Battle::LaneInfo &lane, Battle::NoteData &note)
    {
        if (note.is_hold)
        {
            syscall.template create_entity<Lane, NoteSpeed, Timing, TimingEnd, HoldActive>(
                    Lane{lane.lane_number},
                    NoteSpeed{rhythm_state.note_speed},
                    Timing{note.timing},
                    TimingEnd{note.timing_end});
        }
        else
        {
            syscall.template create_entity<Lane, NoteSpeed, Timing, TimingEnd, HoldActive>(
                    Lane{lane.lane_number}, NoteSpeed{rhythm_state.note_speed}, Timing{note.timing}, TimingEnd{0});
        }
    }

    template<typename T>
    void load_notes(
            T &syscall,
            System::ECS::Query<Battle::ChartData> &query,
            System::ECS::Query<Battle::BattleState> &query2,
            System::ECS::Query<Battle::RhythmState> &query3)
    {
        constexpr float lookahead = 5;

        auto &battle_state = query2.front().get<Battle::BattleState>();
        auto &chart_data = query.front().get<Battle::ChartData>();
        auto &rhythm_state = query3.front().get<Battle::RhythmState>();
        const auto &clock = battle_state.clock_time;

        // repeat for each lane
        for (auto &[lanes] = chart_data; Battle::LaneInfo &lane: lanes)
        {
            if (lane.current_note == lane.notes.size())
            {
                // No work in this lane, move to the next lane
                continue;
            }

            auto &note = lane.notes[lane.current_note];
            while (note.timing < clock + lookahead)
            {
                if (note.timing >= clock)
                {
                    create_note_entity<T>(syscall, rhythm_state, lane, note);
                }

                // increment cursor
                ++lane.current_note;
                if (lane.current_note == lane.notes.size())
                {
                    break;
                }
                note = lane.notes[lane.current_note];
            }
        }
    }
} // namespace Game::Rhythm
