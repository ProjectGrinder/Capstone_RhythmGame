#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void create_note_entity(
            T &syscall,
            Battle::RhythmState &rhythm_state,
            Battle::LaneInfo &lane,
            Battle::NoteData &note)
    {
        if (note.is_hold)
        {
            syscall.template create_entity<Lane, NoteSpeed, Timing, TimingEnd, HoldActive, NoteType>(
                    Lane{lane.lane_number},
                    NoteSpeed{rhythm_state.note_speed},
                    Timing{note.timing},
                    TimingEnd{note.timing_end},
                    HoldActive{false},
                    NoteType{note.note_type});
        }
        else
        {
            syscall.template create_entity<Lane, NoteSpeed, Timing, TimingEnd, HoldActive, NoteType>(
                    Lane{lane.lane_number},
                    NoteSpeed{rhythm_state.note_speed},
                    Timing{note.timing},
                    TimingEnd{0},
                    HoldActive{false},
                    NoteType{note.note_type});
        }
    }

    template<typename T>
    void load_notes(T &syscall,
        System::ECS::Query<Battle::ChartData> &chart_query,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query)
    {
        if (chart_query.begin() == chart_query.end())
        {
            LOG_ERROR("No ChartData found!");
            return;
        }
        if (battle_query.begin() == battle_query.end())
        {
            LOG_ERROR("No BattleState entity found!");
            return;
        }
        if (rhythm_query.begin() == rhythm_query.end())
        {
            LOG_ERROR("No RhythmState entity found!");
            return;
        }

        constexpr int lookahead = 200;

        auto &battle_state = battle_query.front().get<Battle::BattleState>();
        auto &chart_data = chart_query.front().get<Battle::ChartData>();
        auto &rhythm_state = rhythm_query.front().get<Battle::RhythmState>();
        const auto &clock = battle_state.clock_time / 1000;

        // repeat for each lane
        for (auto &lane: chart_data.lanes)
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
