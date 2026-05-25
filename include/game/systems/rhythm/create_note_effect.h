#pragma once

#include "game/components.h"

extern "C" long double get_delta_time();

namespace Game::Rhythm
{
    struct NoteEffect
    {
        float expand_time = 150.00f;
    };

    template<typename T>
    void create_note_effect(
        T &syscall,
        const int lane,
        const Judge judge)
    {
        float x_pos;
        switch (lane)
        {
            case 0:
                x_pos = LANE1;
                break;
            case 1:
                x_pos = LANE2;
                break;
            case 2:
                x_pos = LANE3;
                break;
            case 3:
                x_pos = LANE4;
                break;
            default:
                x_pos = 0;
        }

        Math::Color color;
        switch (judge)
        {
            case PERFECT:
                color = {1, 1, 0, 1};
                break;
            case GREAT:
                color = {0, 1, 0.5, 1};
                break;
            case FINE:
                color = {0.5, 1, 1, 1};
                break;
            default:
                color = {1, 0, 0, 1}; // not used
        }

        syscall.template create_entity<
            NoteEffect,
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            NoteEffect(),
            Render::Sprite{
            .sp = get_assets_record_ptr(get_assets_id("Square")),
            .pos = {{x_pos, JUDGE_LEVEL, 0},{x_pos, JUDGE_LEVEL, 0},{x_pos, JUDGE_LEVEL, 0},{x_pos, JUDGE_LEVEL, 0}},
            .color = color,
            .layer = 10},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
    }

    template<typename T>
    void handle_note_effect(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<NoteEffect, Render::Sprite, Render::Material, Render::Transform> &query)
    {
        using Sprite = Render::Sprite;

        const auto delta_time = static_cast<float>(get_delta_time());
        for (auto &[id, comp] : query)
        {
            if (comp.get<NoteEffect>().expand_time > 0)
            {
                constexpr float expand_time = 150.00f;
                const float delta_x = (NOTE_WIDTH + 10) / 2 * (delta_time / expand_time);
                const float delta_y = (NOTE_HEIGHT + 10) / 2 * (delta_time / expand_time);

                comp.get<Sprite>().pos[0].x -= delta_x;
                comp.get<Sprite>().pos[0].y += delta_y;

                comp.get<Sprite>().pos[1].x += delta_x;
                comp.get<Sprite>().pos[1].y += delta_y;

                comp.get<Sprite>().pos[2].x += delta_x;
                comp.get<Sprite>().pos[2].y -= delta_y;

                comp.get<Sprite>().pos[3].x -= delta_x;
                comp.get<Sprite>().pos[3].y -= delta_y;

                comp.get<NoteEffect>().expand_time -= delta_time;
            }
            else
            {
                constexpr float fade_time = 400.00f;
                comp.get<Sprite>().color.a -= delta_time / fade_time;
                if (comp.get<Sprite>().color.a <= 0)
                    syscall.remove_entity(id);
            }
        }
    }
}