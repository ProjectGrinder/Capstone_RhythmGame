#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;

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
        const auto speed = rhythm_query.front().get<Battle::RhythmState>().base_speed;
        const int render_offset = static_cast<int>((field.spawn_level - field.judge_level) / speed);
        const auto clock_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;

        // notes always move in constant speed
        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state == 1)
            {
                // modify to what seems fit
                comp.get<Position>().y -= speed * frame_time;
            }
            else if (comp.get<NoteStatus>().state == -1)
            {
                //check if note is in render timing
                if (comp.get<Timing>().timing - clock_time <= render_offset)
                {
                    comp.get<NoteStatus>().state = 1;
                    comp.get<Position>().y = field.spawn_level;
                    int lane = comp.get<Timing>().lane;
                    if (lane == 0)
                    {
                        comp.get<Position>().x = field.lane1_spawn;
                    }
                    else if (lane == 1)
                    {
                        comp.get<Position>().x = field.lane2_spawn;
                    }
                    else if (lane == 2)
                    {
                        comp.get<Position>().x = field.lane3_spawn;
                    }
                    else if (lane == 3)
                    {
                        comp.get<Position>().x = field.lane4_spawn;
                    }
                }
            }
        }
    }
}