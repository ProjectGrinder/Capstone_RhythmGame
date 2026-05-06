#pragma once

namespace Game::Battle
{
    template<typename T>
    void create_end_ui(
        T &syscall,
        System::ECS::Query<BattleState> &battle_query)
    {
        const auto battle_state = battle_query.front().get<BattleState>();
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
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "HP", .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = std::to_string(battle_state.hp), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-100, 100, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "Score", .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = std::to_string(battle_state.score), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-100, 50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "PERFECT=" + std::to_string(battle_state.judgement_count.perfect_count), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "GREAT=" + std::to_string(battle_state.judgement_count.great_count), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "FINE=" + std::to_string(battle_state.judgement_count.fine_count), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "MISS=" + std::to_string(battle_state.judgement_count.miss_count), .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, -50, 0}, 0, 0, 0});
        syscall.template create_entity<
            Render::Text,
            Render::Material,
            Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-Normal")), .text = "PRESS_ESC_TO_RETURN_TO_MENU", .color = Math::Color{0, 1, 1, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{-200, -100, 0}, 0, 0, 0});
    }
    template<typename T>
    void check_player_state(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        const auto battle_state = battle_query.front().get<BattleState>();

        if (battle_state.player_state == PlayerState::DEAD || battle_state.player_state == PlayerState::FINISH)
            return;

        if (battle_state.clock_time >= 0)
        {
            if (battle_state.hp > 0)
            {
                create_end_ui(syscall, battle_query);
                battle_query.front().get<BattleState>().player_state = PlayerState::DEAD;
            }
            // else
            // {
            //     battle_query.front().get<BattleState>().player_state = PlayerState::DEAD;
            // }
            return;
        }
        // TODO: If the level is finished with HP > 0, set state to FINISH to show a different text
        /*
        if (music finished)
        {
            render "clear" and "return" assets
            battle_query.front().get<BattleState>().player_state = PlayerState::FINISH;
        }
        */
    }
}
