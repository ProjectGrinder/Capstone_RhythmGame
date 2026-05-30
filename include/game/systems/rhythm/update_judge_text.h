#pragma once

#include "game/components.h"

extern "C" long double get_delta_time();

namespace Game::Rhythm
{
    inline void set_judge(
        const Judge judge,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &query)
    {
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
            comp.get<Render::Sprite>().u0 = u0;
            comp.get<Render::Sprite>().u1 = u1;
            comp.get<Render::Sprite>().v0 = 0;
            comp.get<Render::Sprite>().v1 = 0.25;
            break;

        case GREAT :
            comp.get<Render::Sprite>().u0 = u0;
            comp.get<Render::Sprite>().u1 = u1;
            comp.get<Render::Sprite>().v0 = 0.25;
            comp.get<Render::Sprite>().v1 = 0.5;
            break;

        case FINE :
            comp.get<Render::Sprite>().u0 = u0;
            comp.get<Render::Sprite>().u1 = u1;
            comp.get<Render::Sprite>().v0 = 0.5;
            comp.get<Render::Sprite>().v1 = 0.75;
            break;

        case MISS :
            comp.get<Render::Sprite>().u0 = u0;
            comp.get<Render::Sprite>().u1 = u1;
            comp.get<Render::Sprite>().v0 = 0.75;
            comp.get<Render::Sprite>().v1 = 1;
            break;

        default:
            comp.get<Render::Material>().visible = false;
            return;
        }
        comp.get<JudgeText>().timer = 1000000; //1-second timer
        comp.get<Render::Material>().visible = true;
    }
    template<typename T>
    void update_judge_text(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<JudgeText, Render::Sprite, Render::Material> &query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        // if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::RHYTHM)
        //     return;

        if (query.begin() == query.end())
            return;

        auto comp = query.front();

        if (comp.get<JudgeText>().timer <= 0)
        {
            comp.get<Render::Material>().visible = false;
            return;
        }
        comp.get<JudgeText>().timer -= static_cast<int>(get_delta_time() * 1000);
    }
}
