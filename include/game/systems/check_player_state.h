#pragma once

namespace Game::Battle
{
    template<typename T>
    void check_player_state(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<BattleState> &battle_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        auto battle_state = battle_query.front().get<BattleState>();

        if (battle_state.player_state == PlayerState::DEAD || battle_state.player_state == PlayerState::FINISH)
            return;

        if (battle_state.clock_time >= 0)
        {
            if (battle_state.hp > 0)
            {
                battle_query.front().get<BattleState>().player_state = PlayerState::PLAY;
            }
            else
            {
                syscall.template create_entity<
                Render::Sprite,
                Render::Material,
                Render::Transform>(
                    Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("fail")), .pos = {{-640, 360, 0}, {640, 360, 0}, {640, -360, 0}, {-640, -360, 0}}, .layer = 10},
                    Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
                    Render::Transform{Math::Point{0, 0, 0}, 0, 0, 0});
                syscall.template create_entity<
                    Render::Sprite,
                    Render::Material,
                    Render::Transform>(
                        Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("return")), .pos = {{-320, 180, 0}, {320, 180, 0}, {320, -180, 0}, {-320, -180, 0}}, .layer = 10},
                        Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
                        Render::Transform{Math::Point{0, -240, 0}, 0, 0, 0});
                battle_query.front().get<BattleState>().player_state = PlayerState::DEAD;
            }
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
