#pragma once
#include "utils/constant.h"
namespace Game::Battle
{
    template<typename T>
    void phase_border_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<Border, Render::Sprite> &border_query,
        System::ECS::Query<Backdrop, Render::Sprite> &backdrop_query
        )
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (border_query.begin() == border_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        auto &border_spr = border_query.front().get<Render::Sprite>();
        const auto &border_c = border_query.front().get<Border>();
        auto &back_spr = backdrop_query.front().get<Render::Sprite>();
        const auto &back_c = backdrop_query.front().get<Backdrop>();

        if (battle_state.clock_time <= 0)
        {
            // Lerp quad
            auto &border_pos = border_spr.pos;
            auto &back_pos = back_spr.pos;
            auto quad_box = [&border_pos, &back_pos, &border_c, &back_c](int i)
            {
                border_pos[i].x += (BOX_BH_POS[i].x - border_pos[i].x) /border_c.init_reduce_smooth_factor;
                border_pos[i].y += (BOX_BH_POS[i].y - border_pos[i].y) /border_c.init_reduce_smooth_factor;
                back_pos[i].x += (BOX_BH_POS[i].x - back_pos[i].x) /back_c.init_reduce_smooth_factor;
                back_pos[i].y += (BOX_BH_POS[i].y - back_pos[i].y) /back_c.init_reduce_smooth_factor;
            };

            for (int i=0;i<4;i++) quad_box(i);
        }

        for (auto &[id, comps] : transition_query)
        {
            const auto &transition_data = comps.get<TransitionData>();
            auto &border_pos = border_spr.pos;
            auto &back_pos = back_spr.pos;
            const float duration = static_cast<float>(transition_data.duration)/3;
            auto lerp_box = [&border_pos, &back_pos, &transition_data, &battle_state, &duration](int i, const Math::Point* from, const Math::Point* to)
            {
                float t = static_cast<float>((battle_state.clock_time/1000 - transition_data.timing_start - duration)/duration);
                border_pos[i].x = from[i].x + (to[i].x - from[i].x) * t;
                border_pos[i].y = from[i].y + (to[i].y - from[i].y) * t;
                back_pos[i].x = from[i].x + (to[i].x - from[i].x) * t;
                back_pos[i].y = from[i].y + (to[i].y - from[i].y) * t;
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
        System::ECS::Query<BulletHell::Player, Render::Transform, Render::Sprite, Render::Material, Render::Animation_Controller> &player_query,
        System::ECS::Query<BulletHell::PlayerHitbox, Render::Transform, Render::Material> &player_hitbox_query
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
                    player_query.front().get<Render::Transform>().position = Math::Point(0,-240);
                    player_hitbox_query.front().get<Render::Transform>().position = Math::Point(0,-240);
                    player_query.front().get<Render::Material>().visible = true;
                    player_hitbox_query.front().get<Render::Material>().visible = true;
                    player_query.front().get<BulletHell::Player>().is_active = true;
                    player_spr.color.a = t;
                }

                else if (transition_data.phase == RHYTHM)
                {
                    if (player_query.front().get<BulletHell::Player>().is_active)
                        player_query.front().get<Render::Animation_Controller>().to_id = 0;
                    player_query.front().get<BulletHell::Player>().is_active = false;
                    player_spr.color.a = 1 - t;
                }
            }
            else if (transition_data.state == 2)
            {
                // Shut down
                if (transition_data.phase == RHYTHM)
                {
                    player_query.front().get<Render::Material>().visible = false;
                    player_hitbox_query.front().get<Render::Material>().visible = false;
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
                pos[0] = {-JUDGE_LINE_WIDTH*(1-t),JUDGE_LINE_HEIGHT,0};
                pos[1] = {JUDGE_LINE_WIDTH*(1-t),JUDGE_LINE_HEIGHT,0};
                pos[2] = {JUDGE_LINE_WIDTH*(1-t),-JUDGE_LINE_HEIGHT,0};
                pos[3] = {-JUDGE_LINE_WIDTH*(1-t),-JUDGE_LINE_HEIGHT,0};
                judgement_spr.color.a = (1-t);
            }
            else if (transition_data.state == 1 && transition_data.phase == RHYTHM)
            {
                pos[0] = {-JUDGE_LINE_WIDTH*t,JUDGE_LINE_HEIGHT,0};
                pos[1] = {JUDGE_LINE_WIDTH*t,JUDGE_LINE_HEIGHT,0};
                pos[2] = {JUDGE_LINE_WIDTH*t,-JUDGE_LINE_HEIGHT,0};
                pos[3] = {-JUDGE_LINE_WIDTH*t,-JUDGE_LINE_HEIGHT,0};
                judgement_spr.color.a = t;

            }
        }
    }

    template<typename T>
    void phase_split_line(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<Rhythm::LaneLine, Render::Sprite> &line_query
        )
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (line_query.begin() == line_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();

        for (auto &[id, comps] : line_query)
        {
            auto &line_spr = comps.get<Render::Sprite>();
            auto &pos = line_spr.pos;

            for (auto &[id2, comps2] : transition_query)
            {
                const auto &transition_data = comps2.get<TransitionData>();
                const float duration = static_cast<float>(transition_data.duration) * 1 / 3;
                const float t = (battle_state.clock_time/1000 - transition_data.timing_start - duration*(3-transition_data.state))/duration;
                // Changing state
                if (transition_data.state == 3 && transition_data.phase == BULLET_HELL)
                {
                    pos[0].y = HALF_HEIGHT*2*(1-t);
                    pos[1].y = HALF_HEIGHT*2*(1-t);
                }
                else if (transition_data.state == 1 && transition_data.phase == RHYTHM)
                {
                    pos[0].y = HALF_HEIGHT*2*t;
                    pos[1].y = HALF_HEIGHT*2*t;
                }
            }
        }
    }
} // namespace Game::Battle
