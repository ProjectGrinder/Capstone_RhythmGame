#pragma once
#include "game/components.h"

namespace Game::Rhythm
{
    using Position = Physics::Position;

    inline Position field_to_position(const int lane, const NoteField &field)
    {
        if (lane == 0)
            return Position{field.lane1_spawn, field.spawn_level};
        if (lane == 1)
            return Position{field.lane2_spawn, field.spawn_level};
        if (lane == 2)
            return Position{field.lane3_spawn, field.spawn_level};
        if (lane == 3)
            return Position{field.lane4_spawn, field.spawn_level};
        return Position{0, 0};
    }

    template<typename T>
    void create_note_entity(
            T &syscall,
            Battle::LaneInfo &lane,
            Battle::NoteData &note,
            NoteField &field)
    {
        if (note.is_hold)
        {
            syscall.template create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                    field_to_position(lane.lane_number, field),
                    Timing{lane.lane_number, note.timing},
                    HoldStart{true},
                    NoteType{note.note_type},
                    NoteStatus{-1});
            syscall.template create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                    field_to_position(lane.lane_number, field),
                    Timing{lane.lane_number, note.timing_end},
                    HoldStart{false},
                    NoteType{-1},
                    NoteStatus{-1});
        }
        else
        {
            syscall.template create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                    field_to_position(lane.lane_number, field),
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
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<NoteField> &field_query)
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
        auto field = field_query.front().get<NoteField>();

        // repeat for each lane
        for (auto &lane: lanes)
        {
            while (lane.current_note < lane.notes.size())
            {
                auto &note = lane.notes.at(lane.current_note);
                create_note_entity<T>(syscall, lane, note, field);
                ++lane.current_note;
            }
        }
    }
} // namespace Game::Rhythm
