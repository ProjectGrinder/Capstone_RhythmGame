#pragma once

#include "game/components.h"
extern "C" long double get_delta_time();

namespace Game::Rhythm
{
    using Transform = Render::Transform;
    using Material = Render::Material;
    using Sprite = Render::Sprite;

    inline void handle_speed_change(
        const float render_offset,
        const int clock_time,
        System::ECS::Query<Transform, Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<HoldConnect, NoteStatus, Transform, Material, Sprite> &hold_query,
        const NoteField &field)
    {
        const auto height = field.spawn_level - field.judge_level;

        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state != -1)
            {
                const auto note_diff = static_cast<float>(comp.get<Timing>().timing - clock_time);
                if (note_diff < render_offset)
                {
                    comp.get<Transform>().position.y = field.judge_level + height * (static_cast<float>(note_diff) / render_offset);
                    comp.get<NoteStatus>().state = 1;
                    comp.get<Material>().visible = true;
                }
                else
                {
                    comp.get<Transform>().position.y = field.spawn_level;
                    comp.get<NoteStatus>().state = 0;
                    comp.get<Material>().visible = false;
                }
            }
        }
        if (hold_query.begin() == hold_query.end()) return;
    }

    template<typename T>
    void update_notes(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Transform, Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<Battle::RhythmState> &rhythm_query,
        System::ECS::Query<NoteField> &field_query,
        System::ECS::Query<HoldConnect, NoteStatus, Transform, Material, Sprite> &hold_query)
    {
        if (note_query.begin() == note_query.end())
            return;

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        if (rhythm_query.begin() == rhythm_query.end())
            return;

        // const auto frame_time = get_delta_time();
        // if (frame_time == 0)
        //     return;

        const auto frame_time = get_delta_time();
        const auto field = field_query.front().get<NoteField>();
        const auto current_speed = rhythm_query.front().get<Battle::RhythmState>().current_speed;
        const auto height = field.spawn_level - field.judge_level;
        const float render_offset = field.move_time / current_speed; // speed 1 -> 5000 ms
        const auto clock_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;

        // If speed change is detected, change note positions
        if (rhythm_query.front().get<Battle::RhythmState>().speed_change)
        {
            handle_speed_change(render_offset, clock_time, note_query, hold_query, field);
            rhythm_query.front().get<Battle::RhythmState>().speed_change = false;
            return;
        }
        // Notes always move in constant speed
        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state == 1)
            {
                comp.get<Transform>().position.y -= (height / render_offset) * static_cast<float>(frame_time);
            }
            else if (comp.get<NoteStatus>().state == 0)
            {
                //check if note is in render timing
                if (static_cast<float>(comp.get<Timing>().timing - clock_time) <= render_offset)
                {
                    comp.get<NoteStatus>().state = 1;
                    comp.get<Material>().visible = true;
                }
            }
        }
        for (auto &[id, comp]: hold_query)
        {
            // for hold parts. state (-1) will still render until the length is 0
            // hold parts will not go pass judge level
            // if hit earlier than 0 ms, snap the hold part to judge level
            if (comp.get<NoteStatus>().state != 0)
            {
                const auto judge_level = field.judge_level;
                // handle start
                if (comp.get<Transform>().position.y > judge_level)
                {
                    if (comp.get<HoldConnect>().hit)
                    {
                        comp.get<Transform>().position.y = judge_level;
                    }
                    else
                    {
                        comp.get<Transform>().position.y -= (height / render_offset) * static_cast<float>(frame_time);
                    }
                }
                else
                {
                    comp.get<Transform>().position.y = judge_level;
                }
                // handle end
                // 1. stretch from the top
                // 2. if short enough, only move along
                // 3. when start is at judge level, shrink down
                auto end_diff = static_cast<float>(comp.get<HoldConnect>().timing_end - clock_time);
                float current_height = height * (end_diff / render_offset);
                constexpr float default_height = 960.0f;
                if (end_diff == 0)
                {
                    // hold part ends, stop rendering
                    comp.get<Sprite>().pos[0].y = 0;
                    comp.get<Sprite>().pos[1].y = 0;
                    comp.get<Material>().visible = false;
                    continue;
                }
                if (comp.get<HoldConnect>().timing_start <= clock_time)
                {
                    if (end_diff > render_offset)
                    {
                        // case 3
                        current_height = height;
                    }
                }
                else
                {
                    if (end_diff > render_offset)
                    {
                        // case 1
                        const auto time_diff = render_offset - static_cast<float>(comp.get<HoldConnect>().timing_start - clock_time);
                        current_height = height * (time_diff / render_offset);
                    }
                    else continue;
                    // case 2 - only transform, no need to change sprite
                }
                const float uv = current_height / default_height;
                comp.get<Sprite>().pos[0].y = current_height;
                comp.get<Sprite>().pos[1].y = current_height;
                comp.get<Sprite>().v1 = uv;
            }
            else
            {
                //check if note is in render timing
                if (static_cast<float>(comp.get<HoldConnect>().timing_start - clock_time) <= render_offset)
                {
                    comp.get<NoteStatus>().state = 1;
                    comp.get<Material>().visible = true;
                }
            }
        }
    }
}