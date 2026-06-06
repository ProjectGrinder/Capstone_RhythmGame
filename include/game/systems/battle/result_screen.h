#pragma once

namespace Game::Battle
{
    // enum Rank
    // {
    //     P,S,AA,A,BB,B,CC,C,D,F
    // }; // double characters mean "+" (A+)

    inline Render::Sprite get_rank_sprite(
        const int hit_count,
        const int graze,
        const float accuracy,
        const PlayerState player_state)
    {
        float graze_score = static_cast<float>(3*graze)/20;
        if (graze_score > 3)
            graze_score = 3;

        float accuracy_score = 5 - (10000 - accuracy)/200;
        if (accuracy_score < 0)
            accuracy_score = 0;

        auto grade_spr = Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("rank")), .pos = {{-50, 50, 0}, {50, 50, 0}, {50, -50, 0}, {-50, -50, 0}}, .layer = 51};

        const float final_score = graze_score + accuracy_score - (static_cast<float>(hit_count) * 0.25f);
        if (final_score >= 8) // P
        {
            grade_spr.u0 = 0;
            grade_spr.v0 = 0;
            grade_spr.u1 = 0.2f;
            grade_spr.v1 = 0.5f;
        }
        else if (final_score >= 7.5) // S
        {
            grade_spr.u0 = 0;
            grade_spr.v0 = 0.5f;
            grade_spr.u1 = 0.2f;
            grade_spr.v1 = 1;
        }
        else if (final_score >= 7) // A+
        {
            grade_spr.u0 = 0.2f;
            grade_spr.v0 = 0;
            grade_spr.u1 = 0.4f;
            grade_spr.v1 = 0.5f;
        }
        else if (final_score >= 6.5) // A
        {
            grade_spr.u0 = 0.2f;
            grade_spr.v0 = 0.5f;
            grade_spr.u1 = 0.4f;
            grade_spr.v1 = 1;
        }
        else if (final_score >= 6) // B+
        {
            grade_spr.u0 = 0.4f;
            grade_spr.v0 = 0;
            grade_spr.u1 = 0.6f;
            grade_spr.v1 = 0.5f;
        }
        else if (final_score >= 5) // B
        {
            grade_spr.u0 = 0.4f;
            grade_spr.v0 = 0.5f;
            grade_spr.u1 = 0.6f;
            grade_spr.v1 = 1;
        }
        else if (final_score >= 4) // C+
        {
            grade_spr.u0 = 0.6f;
            grade_spr.v0 = 0;
            grade_spr.u1 = 0.8f;
            grade_spr.v1 = 0.5f;
        }
        else if (final_score >= 3) // C
        {
            grade_spr.u0 = 0.6f;
            grade_spr.v0 = 0.5f;
            grade_spr.u1 = 0.8f;
            grade_spr.v1 = 1;
        }
        else if (final_score <= 0 && player_state == FINISH) // F
        {
            grade_spr.u0 = 0.8f;
            grade_spr.v0 = 0.5f;
            grade_spr.u1 = 1;
            grade_spr.v1 = 1;
        }
        else // D
        {
            grade_spr.u0 = 0.8f;
            grade_spr.v0 = 0;
            grade_spr.u1 = 1;
            grade_spr.v1 = 0.5f;
        }
        return grade_spr;
    }
    template<typename T>
    void create_result_ui(
        T &syscall,
        System::ECS::Query<BattleState, BulletHellState, RhythmState> &state_query)
    {
        const auto battle_state = state_query.front().get<BattleState>();
        const auto rhythm_state = state_query.front().get<RhythmState>();
        const auto bullet_state = state_query.front().get<BulletHellState>();
        const float accept_percent = static_cast<float>(battle_state.current_accept)/static_cast<float>(battle_state.max_accept_gauge);

        auto result_text = Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "", .color = Math::Color{0, 0, 0, 1}, .layer = 100};
        if (battle_state.player_state == DEAD)
        {
            result_text.text = "LOST";
            result_text.color = Math::Color{1, 0, 0, 1};
        }
        else if (accept_percent >= 0.7f)
        {
            result_text.text = "WIN";
            result_text.color = Math::Color{0, 1, 0, 1};
        }
        else
        {
            result_text.text = "DRAW";
            result_text.color = Math::Color{1, 1, 0, 1};
        }

        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            std::move(result_text),
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{-50, 150, 0, 0, 0, 2, 2, 2});
        syscall.template create_entity<
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-240, 135, 0}, {240, 135, 0}, {240, -135, 0}, {-240, -135, 0}}, .layer = 50},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Hit Taken", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 100, 0}, 0, 0, 0});
        // TODO: change to hit taken variable when added
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "4", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Graze", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = std::to_string(bullet_state.graze), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Accuracy", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = rhythm_state.accuracy_text, .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Perfect " + std::to_string(battle_state.judgement_count.perfect_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Great " + std::to_string(battle_state.judgement_count.great_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Fine " + std::to_string(battle_state.judgement_count.fine_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, -100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Miss " + std::to_string(battle_state.judgement_count.miss_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -100, 0}, 0, 0, 0});
        // syscall.template create_entity<
        //     Render::Sprite,
        //     Render::Material,
        //     Render::Transform>
        // (
        //     Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("return")), .pos = {{-160, 90, 0}, {160, 90, 0}, {160, -90, 0}, {-160, -90, 0}}, .layer = 51},
        //     Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        //     Render::Transform{Math::Point{0, -100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            get_rank_sprite(4, bullet_state.graze, rhythm_state.accuracy, battle_state.player_state),
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{170, 70, 0}, 0, 0, 0});
    }
}