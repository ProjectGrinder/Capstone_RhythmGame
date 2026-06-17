#pragma once

#include "game/components.h"
#include "game/utils/constant.h"

namespace Game::Rhythm
{
    using Transform = Render::Transform;
    using Material = Render::Material;
    using Sprite = Render::Sprite;

    inline void adjust_hold(
        const float default_height,
        const float start_pos,
        const float end_pos,
        System::ECS::Query<HoldConnect, NoteStatus, Transform, Material, Sprite>::StoredTuple *comp)
    {
        const float current_height = end_pos - start_pos;
        const float uv = current_height / default_height;
        comp->get<Sprite>().pos[0].y = current_height;
        comp->get<Sprite>().pos[1].y = current_height;
        comp->get<Sprite>().v1 = uv;
    }

    inline void handle_speed_change(
        const float render_offset,
        const int clock_time,
        System::ECS::Query<Transform, Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<HoldConnect, NoteStatus, Transform, Material, Sprite> &hold_query,
        const float hold_height)
    {
        constexpr auto height = SPAWN_LEVEL - JUDGE_LEVEL;

        for (auto &[id, comp]: note_query)
        {
            if (comp.get<NoteStatus>().state != -1)
            {
                const auto note_diff = static_cast<float>(comp.get<Timing>().timing - clock_time);
                if (note_diff < render_offset)
                {
                    comp.get<Transform>().position.y = JUDGE_LEVEL + height * (static_cast<float>(note_diff) / render_offset);
                    comp.get<NoteStatus>().state = 1;
                    comp.get<Material>().visible = true;
                }
                else
                {
                    comp.get<Transform>().position.y = SPAWN_LEVEL;
                    comp.get<NoteStatus>().state = 0;
                    comp.get<Material>().visible = false;
                }
            }
        }
        for (auto &[id2, comp2]: hold_query)
        {
            auto end_diff = static_cast<float>(comp2.get<HoldConnect>().timing_end - clock_time);
            if (end_diff <= 0)
                continue;

            auto start_diff = static_cast<float>(comp2.get<HoldConnect>().timing_start - clock_time);
            auto start_pos = height * (start_diff / render_offset);
            float end_pos = height * (end_diff / render_offset);

            // handle start
            if (comp2.get<NoteStatus>().state == -1)
            {
                comp2.get<Transform>().position.y = JUDGE_LEVEL;
                start_pos = 0;
                comp2.get<Material>().visible = true;
            }
            else if (start_diff < render_offset)
            {
                comp2.get<Transform>().position.y = JUDGE_LEVEL + start_pos;
                comp2.get<NoteStatus>().state = 1;
                comp2.get<Material>().visible = true;
            }
            else
            {
                comp2.get<Transform>().position.y = SPAWN_LEVEL;
                comp2.get<Sprite>().pos[0].y = 0;
                comp2.get<Sprite>().pos[1].y = 0;
                comp2.get<Sprite>().v1 = 0;
                comp2.get<NoteStatus>().state = 0;
                comp2.get<Material>().visible = false;
                continue;
            }
            if (end_diff > render_offset)
            {
                end_pos = height;
            }
            adjust_hold(hold_height, start_pos, end_pos, &comp2);
        }
    }

    template<typename T>
    void update_notes(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Transform, Material, Timing, HoldStart, NoteType, NoteStatus> &note_query,
        System::ECS::Query<Battle::BattleState, Battle::RhythmState> &battle_query,
        System::ECS::Query<HoldConnect, NoteStatus, Transform, Material, Sprite> &hold_query)
    {
        if (note_query.begin() == note_query.end())
            return;

        if (battle_query.begin() == battle_query.end())
            return;

        if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
            return;

        const auto frame_time = get_delta_time();
        const auto current_speed = battle_query.front().get<Battle::RhythmState>().current_speed;
        constexpr auto height = SPAWN_LEVEL - JUDGE_LEVEL;
        const float render_offset = NOTE_TIME / current_speed;
        const auto clock_time = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
        constexpr float default_hold_height = 960.0f;

        // If speed change is detected, change note positions
        if (battle_query.front().get<Battle::RhythmState>().speed_change)
        {
            handle_speed_change(render_offset, clock_time, note_query, hold_query, default_hold_height);
            battle_query.front().get<Battle::RhythmState>().speed_change = false;
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
            // state (-1) means already hit/miss
            // hold parts will not go pass judge level
            // if hit earlier than 0 ms, snap the hold part to judge level
            auto end_diff = static_cast<float>(comp.get<HoldConnect>().timing_end - clock_time);

            if (end_diff <= 0)
            {
                if (comp.get<Material>().visible)
                {
                    // hold part ends, stop rendering
                    comp.get<Sprite>().pos[0].y = 0;
                    comp.get<Sprite>().pos[1].y = 0;
                    comp.get<NoteStatus>().state = -1;
                    comp.get<Material>().visible = false;
                }
                continue;
            }

            if (comp.get<NoteStatus>().state == 0 && static_cast<float>(comp.get<HoldConnect>().timing_start - clock_time) <= render_offset)
            {
                comp.get<NoteStatus>().state = 1;
                comp.get<Material>().visible = true;
                continue;
            }

            auto start_pos = height * (static_cast<float>(comp.get<HoldConnect>().timing_start - clock_time)) / render_offset;
            float end_pos = height * (end_diff / render_offset);

            if (comp.get<NoteStatus>().state == -1 || comp.get<Transform>().position.y <= JUDGE_LEVEL)
            {
                comp.get<Transform>().position.y = JUDGE_LEVEL;
                start_pos = 0;
                if (end_diff > render_offset)
                {
                    end_pos = height;
                    // if it's the same full height, skip
                    if (comp.get<Sprite>().v1 == (end_pos - start_pos) / default_hold_height)
                        continue;
                }
                adjust_hold(default_hold_height, start_pos, end_pos, &comp);
            }
            else if (comp.get<NoteStatus>().state == 1)
            {
                // handle start
                if (comp.get<Transform>().position.y > JUDGE_LEVEL)
                {
                    comp.get<Transform>().position.y -= (height / render_offset) * static_cast<float>(frame_time);
                }
                // handle end
                // 1. stretch from the top
                // 2. if short enough, only move along
                // 3. when start is at judge level, shrink down
                if (comp.get<HoldConnect>().timing_start <= clock_time)
                {
                    if (end_diff > render_offset)
                    {
                        // case 3
                        end_pos = height;
                    }
                }
                else
                {
                    if (end_diff > render_offset)
                    {
                        // case 1
                        // const auto time_diff = render_offset - static_cast<float>(comp.get<HoldConnect>().timing_start - clock_time);
                        end_pos = height;
                    }
                    else continue;
                    // case 2 - only transform, no need to change sprite
                }
                adjust_hold(default_hold_height, start_pos, end_pos, &comp);
            }
        }
    }
}