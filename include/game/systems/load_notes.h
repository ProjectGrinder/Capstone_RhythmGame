#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void LoadNotes(T &task_manager, System::ECS::Query<Battle::ChartData> &query, System::ECS::Query<Battle::BattleState> &query2, System::ECS::Query<Battle::RhythmState> &query3)
    {
        constexpr float lookahead = 5;

        auto &battle_state = query2.front().get<Battle::BattleState>();
        auto &chart_data = query.front().get<Battle::ChartData>();
        auto &rhythm_state = query3.front().get<Battle::RhythmState>();
        const auto &clock = battle_state.clock_time;

        // repeat for each lane
        for (auto &[lanes] = chart_data; Battle::LaneInfo & lane: lanes)
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
                        task_manager.template create_entity<Lane, NoteSpeed, Timing, TimingEnd>(
                            Lane{lane.lane_number},
                            NoteSpeed{rhythm_state.note_speed},
                            Timing{note.timing},
                            TimingEnd{note.timing_end});
                    }
                    else
                    {
                        task_manager.template create_entity<Lane, NoteSpeed, Timing>(
                            Lane{lane.lane_number},
                            NoteSpeed{rhythm_state.note_speed},
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
