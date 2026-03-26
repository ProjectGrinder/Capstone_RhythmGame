#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void create_note_entity(
            T &syscall,
            Battle::LaneInfo &lane,
            Battle::NoteData &note)
    {
        if (note.is_hold)
        {
            syscall.template create_entity<Timing, HoldStart, NoteType, NoteStatus>(
                    Timing{lane.lane_number, note.timing},
                    HoldStart{true},
                    NoteType{note.note_type},
                    NoteStatus{-1});
            syscall.template create_entity<Timing, HoldStart, NoteType, NoteStatus>(
                    Timing{lane.lane_number, note.timing_end},
                    HoldStart{false},
                    NoteType{-1},
                    NoteStatus{-1});
        }
        else
        {
            syscall.template create_entity<Timing, HoldStart, NoteType, NoteStatus>(
                    Timing{lane.lane_number, note.timing},
                    HoldStart{false},
                    NoteType{note.note_type},
                    NoteStatus{-1});
        }
    }

    template<typename T>
    void load_chart(T &syscall,
        System::ECS::Query<Battle::ChartData> &chart_query,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query)
    {
        if (chart_query.begin() == chart_query.end())
        {
            return;
        }
        if (battle_query.begin() == battle_query.end())
        {
            return;
        }
        if (rhythm_query.begin() == rhythm_query.end())
        {
            return;
        }

        auto &[lanes] = chart_query.front().get<Battle::ChartData>();

        // repeat for each lane
        for (auto &lane: lanes)
        {
            while (lane.current_note < lane.notes.size())
            {
                auto &note = lane.notes.at(lane.current_note);
                create_note_entity<T>(syscall, lane, note);
                ++lane.current_note;
            }
        }
    }
} // namespace Game::Rhythm
