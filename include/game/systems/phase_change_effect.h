#pragma once
#include "utils/constant.h"
namespace Game::Battle
{
    template<typename T>
    void phase_border_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<Border, Render::Sprite> &border_query
        )
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (border_query.begin() == border_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        auto &border_spr = border_query.front().get<Render::Sprite>();
        const auto &border_c = border_query.front().get<Border>();

        if (battle_state.clock_time <= 0)
        {
            // Lerp quad
            auto &pos = border_spr.pos;
            auto quad_box = [&pos, &border_c](int i)
            {
                pos[i].x += (BOX_BH_POS[i].x - pos[i].x) * static_cast<float>(get_delta_time())/border_c.init_reduce_smooth_factor;
                pos[i].y += (BOX_BH_POS[i].y - pos[i].y) * static_cast<float>(get_delta_time())/border_c.init_reduce_smooth_factor;
            };

            for (int i=0;i<4;i++) quad_box(i);
        }

        for (auto &[id, comps] : transition_query)
        {
            const auto &transition_data = comps.get<TransitionData>();
            auto &pos = border_spr.pos;
            const float duration = static_cast<float>(transition_data.duration)/3;
            auto lerp_box = [&pos, &transition_data, &battle_state, &duration](int i, const Math::Point* from, const Math::Point* to)
            {
                float t = static_cast<float>((battle_state.clock_time/1000 - transition_data.timing_start - duration)/duration);
                pos[i].x = from[i].x + (to[i].x - from[i].x) * t;
                pos[i].y = from[i].y + (to[i].y - from[i].y) * t;
            };
            // Changing state
            if (transition_data.state == 2)
            {
                if (transition_data.phase == BULLET_HELL)
                    for (int i=0;i<4;i++) lerp_box(i, BOX_RG_POS, BOX_BH_POS);
                else if (transition_data.phase == RHYTHM)
                    for (int i=0;i<4;i++) lerp_box(i, BOX_BH_POS, BOX_RG_POS);
            }
        }
    }

    template<typename T>
    void phase_player_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<BulletHell::Player, Render::Sprite> &player_query
        )
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (player_query.begin() == player_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        auto &player_spr = player_query.front().get<Render::Sprite>();

        for (auto &[id, comps] : transition_query)
        {
            const auto &transition_data = comps.get<TransitionData>();
            const float duration = static_cast<float>(transition_data.duration) * 1 / 3;
            const float t = static_cast<float>((battle_state.clock_time/1000 - transition_data.timing_start)/duration);
            // Changing state
            if (transition_data.state == 3)
            {
                if (transition_data.phase == BULLET_HELL)
                {
                    player_query.front().get<BulletHell::Player>().is_active = true;
                    player_spr.color.a = t;
                }

                else if (transition_data.phase == RHYTHM)
                {
                    player_query.front().get<BulletHell::Player>().is_active = false;
                    player_spr.color.a = 1 - t;
                }

            }
        }
    }

    template<typename T>
    void phase_judgement_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<Rhythm::JudgementLine, Render::Sprite> &judgement_query
        )
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (judgement_query.begin() == judgement_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        auto &judgement_spr = judgement_query.front().get<Render::Sprite>();
        auto &pos = judgement_spr.pos;

        for (auto &[id, comps] : transition_query)
        {
            const auto &transition_data = comps.get<TransitionData>();
            const float duration = static_cast<float>(transition_data.duration) * 1 / 3;
            const float t = (battle_state.clock_time/1000 - transition_data.timing_start - duration*(3-transition_data.state))/duration;
            // Changing state
            if (transition_data.state == 3 && transition_data.phase == BULLET_HELL)
            {
                pos[0] = {-500*(1-t),5,0};
                pos[1] = {500*(1-t),5,0};
                pos[2] = {500*(1-t),-5,0};
                pos[3] = {-500*(1-t),-5,0};
                judgement_spr.color.a = (1-t);
            }
            else if (transition_data.state == 1 && transition_data.phase == RHYTHM)
            {
                pos[0] = {-500*t,5,0};
                pos[1] = {500*t,5,0};
                pos[2] = {500*t,-5,0};
                pos[3] = {-500*t,-5,0};
                judgement_spr.color.a = t;

            }
        }
    }
} // namespace Game::Battle
