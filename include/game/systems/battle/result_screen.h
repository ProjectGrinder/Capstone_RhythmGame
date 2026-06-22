#pragma once

namespace Game::Battle
{
    enum Rank
    {
        P,S,AA,A,BB,B,CC,C,D,F
    }; // double characters mean "+" (A+)

    inline Rank calculate_rank(
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

        const float final_score = graze_score + accuracy_score - (static_cast<float>(hit_count) * 0.25f);
        if (final_score >= 8)
        {
            return P;
        }
        if (final_score >= 7.5)
        {
            return S;
        }
        if (final_score >= 7)
        {
            return AA;
        }
        if (final_score >= 6.5)
        {
            return A;
        }
        if (final_score >= 6)
        {
            return BB;
        }
        if (final_score >= 5)
        {
            return B;
        }
        if (final_score >= 4)
        {
            return CC;
        }
        if (final_score >= 3)
        {
            return C;
        }
        if (final_score <= 0 && player_state == FINISH) // F
        {
            return F;
        }
        return D;
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
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-240, 135, 0}, {240, 135, 0}, {240, -175, 0}, {-240, -175, 0}}, .layer = 50},
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
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = std::to_string(bullet_state.hit_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
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
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Perfect " + std::to_string(battle_state.judgement_count.perfect_count), .color = Math::Color{1, 0.7f, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{-200, -50, 0, 0, 0, 0.8f, 0.8f, 1});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Great " + std::to_string(battle_state.judgement_count.great_count), .color = Math::Color{0, 1, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, -50, 0, 0, 0, 0.8f, 0.8f, 1});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Fine " + std::to_string(battle_state.judgement_count.fine_count), .color = Math::Color{0, 0.7f, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{-200, -100, 0, 0, 0, 0.8f, 0.8f, 1});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Miss " + std::to_string(battle_state.judgement_count.miss_count), .color = Math::Color{1, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, -100, 0, 0, 0, 0.8f, 0.8f, 1});
        syscall.template create_entity<
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("return")), .pos = {{-160, 90, 0}, {160, 90, 0}, {160, -90, 0}, {-160, -90, 0}}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -150, 0}, 0, 0, 0});

        const auto rank = calculate_rank(0, bullet_state.graze, rhythm_state.accuracy, battle_state.player_state);
        syscall.template create_entity<
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("rank")), .pos = {{-50, 50, 0}, {50, 50, 0}, {50, -50, 0}, {-50, -50, 0}},
                .layer = 51, .u0 = 0.1f*static_cast<float>(rank), .u1 = 0.1f*(static_cast<float>(rank)+1)},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{170, 70, 0}, 0, 0, 0});
    }
}