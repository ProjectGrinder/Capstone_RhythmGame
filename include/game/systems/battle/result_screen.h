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
        const float accuracy)
    {
        float graze_score = static_cast<float>(3*graze)/20;
        if (graze_score > 3)
            graze_score = 3;

        float accuracy_score = 5 - (100 - accuracy)/2;
        if (accuracy_score < 0)
            accuracy_score = 0;

        const float final_score = graze_score + accuracy_score - (static_cast<float>(hit_count) * 0.25f);
        if (final_score >= 8)
            return P;
        if (final_score >= 7.5)
            return S;
        if (final_score >= 7)
            return AA;
        if (final_score >= 6.5)
            return A;
        if (final_score >= 6)
            return BB;
        if (final_score >= 5)
            return B;
        if (final_score >= 4)
            return CC;
        if (final_score >= 3)
            return C;
        return D;
    }
    template<typename T>
    void create_result_ui(
        T &syscall,
        System::ECS::Query<BattleState, BulletHellState, RhythmState> &state_query)
    {
        const auto battle_state = state_query.front().get<BattleState>();
        const auto rhythm_state = state_query.front().get<RhythmState>();
        const float accept_percent = static_cast<float>(battle_state.current_accept)/static_cast<float>(battle_state.max_accept_gauge);
        // TODO: Add win/draw/lost text
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
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "HP", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = std::to_string(battle_state.hp), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Accuracy", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = rhythm_state.accuracy_text, .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Perfect " + std::to_string(battle_state.judgement_count.perfect_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Great " + std::to_string(battle_state.judgement_count.great_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Fine " + std::to_string(battle_state.judgement_count.fine_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Miss " + std::to_string(battle_state.judgement_count.miss_count), .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Sprite,
            Render::Material,
            Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("return")), .pos = {{-160, 90, 0}, {160, 90, 0}, {160, -90, 0}, {-160, -90, 0}}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -100, 0}, 0, 0, 0});
    }
}