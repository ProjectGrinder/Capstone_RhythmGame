#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void LoadNotes(T &syscall, System::ECS::Query<Battle::BattleState> &query)
    {
        constexpr float lookahead = 5;

        auto &battle_state = query.front().get<Battle::BattleState>();
        const auto &clock = battle_state.clock_time;

        // repeat for each lane
        for (auto &[lanes] = battle_state.chart_data; Battle::LaneInfo & lane: lanes)
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
                    if (note.is_hold)
                    {
                        syscall.template create_entity<NoteSpeed, Timing, TimingEnd>(
                            NoteSpeed{battle_state.rhythm_state.note_speed},
                            Timing{note.timing},
                            TimingEnd{note.timing_end});
                    }
                    else
                    {
                        syscall.template create_entity<NoteSpeed, Timing>(
                            NoteSpeed{battle_state.rhythm_state.note_speed},
                            Timing{note.timing});
                    }
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
