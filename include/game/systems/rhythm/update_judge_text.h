#pragma once

#include "game/components.h"

extern "C" long double get_delta_time();

namespace Game::Rhythm
{
    inline void set_judge(
        const Judge judge,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &query)
    {
        using Sprite = Render::Sprite;

        if (query.begin() == query.end())
        {
            return;
        }

        auto comp = query.front();
        constexpr float u0 = 0;
        constexpr float u1 = 0.5;
        switch (judge)
        {
        case PERFECT :
            comp.get<Sprite>().u0 = u0;
            comp.get<Sprite>().u1 = u1;
            comp.get<Sprite>().v0 = 0;
            comp.get<Sprite>().v1 = 0.25;
            break;

        case GREAT :
            comp.get<Sprite>().u0 = u0;
            comp.get<Sprite>().u1 = u1;
            comp.get<Sprite>().v0 = 0.25;
            comp.get<Sprite>().v1 = 0.5;
            break;

        case FINE :
            comp.get<Sprite>().u0 = u0;
            comp.get<Sprite>().u1 = u1;
            comp.get<Sprite>().v0 = 0.5;
            comp.get<Sprite>().v1 = 0.75;
            break;

        case MISS :
            comp.get<Sprite>().u0 = u0;
            comp.get<Sprite>().u1 = u1;
            comp.get<Sprite>().v0 = 0.75;
            comp.get<Sprite>().v1 = 1;
            break;

        default:
            comp.get<Render::Material>().visible = false;
            return;
        }
        comp.get<JudgeText>().change = true;
        comp.get<Render::Material>().visible = true;
    }
    template<typename T>
    void update_judge_text(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &query)
    {
        using Sprite = Render::Sprite;

        if (battle_query.begin() == battle_query.end())
            return;

        // if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        //     return;

        if (query.begin() == query.end())
            return;

        auto comp = query.front();
        constexpr float pulse_time = 100.00f;
        constexpr float stay_time = 800.00f;

        if (comp.get<JudgeText>().change)
        {
            constexpr float pulse_x = 140.00f;
            constexpr float pulse_y = 35.00f;

            comp.get<Sprite>().pos[0].x = -pulse_x;
            comp.get<Sprite>().pos[0].y = pulse_y;

            comp.get<Sprite>().pos[1].x = pulse_x;
            comp.get<Sprite>().pos[1].y = pulse_y;

            comp.get<Sprite>().pos[2].x = pulse_x;
            comp.get<Sprite>().pos[2].y = -pulse_y;

            comp.get<Sprite>().pos[3].x = -pulse_x;
            comp.get<Sprite>().pos[3].y = -pulse_y;

            comp.get<JudgeText>().change = false;
            comp.get<JudgeText>().pulse_time = pulse_time;
            comp.get<JudgeText>().stay_time = stay_time;
            return;
        }

        const auto delta_time = static_cast<float>(get_delta_time());
        if (comp.get<JudgeText>().pulse_time > 0)
        {
            const float delta_x = 20 * (delta_time / pulse_time);
            const float delta_y = 5 * (delta_time / pulse_time);

            comp.get<Sprite>().pos[0].x += delta_x;
            comp.get<Sprite>().pos[0].y -= delta_y;

            comp.get<Sprite>().pos[1].x -= delta_x;
            comp.get<Sprite>().pos[1].y -= delta_y;

            comp.get<Sprite>().pos[2].x -= delta_x;
            comp.get<Sprite>().pos[2].y += delta_y;

            comp.get<Sprite>().pos[3].x += delta_x;
            comp.get<Sprite>().pos[3].y += delta_y;

            comp.get<JudgeText>().pulse_time -= delta_time;
        }
        else if (comp.get<JudgeText>().stay_time > 0)
        {
            comp.get<JudgeText>().stay_time -= delta_time;
        }
        else
        {
            comp.get<Render::Material>().visible = false;
        }
    }
}
