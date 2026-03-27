#pragma once

namespace Game::Overview
{
    template<typename T>
    void detect_overlap_interactable([[maybe_unused]] T &syscall,
                        System::ECS::Query<GlobalState> &global_query,
                        System::ECS::Query<Input> &input_query,
                         System::ECS::Query<Interactable, EventState, Physics::Position, Physics::Scale, Physics::CircularCollider> &interactable_query,
                         System::ECS::Query<Player,Physics::Position, Physics::Scale, Physics::CircularCollider> &player_query)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &global_state = global_query.front().components.get<GlobalState>();
        const auto &input = input_query.front().components.get<BulletHell::Input>();

        for (auto &[pid, player_comps] : player_query)
        {
            const auto &player_pos = player_comps.get<Physics::Position>();

            for (auto &[id, comps] : interactable_query)
            {
                const float dx = comps.get<Physics::Position>().x - player_pos.x;
                const float dy = comps.get<Physics::Position>().y - player_pos.y;
                const float distance_squared = dx * dx + dy * dy;

                const float trigger_radius = comps.get<Physics::CircularCollider>().radius_x * comps.get<Physics::Scale>().scaleX;

                if (distance_squared <= trigger_radius*trigger_radius)
                {
                    comps.get<Interactable>().in_range = true;
                    if (input.up_pressed && !global_state.interactionLocked) comps.get<EventState>().event_occupied = true;
                }
            }
        }
    }

    template<typename T>
    void detect_overlap_block([[maybe_unused]] T &syscall,
                         System::ECS::Query<Block, Physics::Position, Physics::Scale, Physics::RectangularCollider> &block_query,
                         System::ECS::Query<Player,Physics::Position, Physics::Scale, Velocity, Physics::CircularCollider> &player_query)
    {
        for (auto &[pid, player_comps] : player_query)
        {
            auto &player_c = player_comps.get<Player>();
            const auto &player_pos = player_comps.get<Physics::Position>();
            const auto &player_col = player_comps.get<Physics::CircularCollider>();
            auto &player_velos = player_comps.get<Velocity>();

            // AABB check
            for (auto &[id, comps] : block_query)
            {
                const auto &pos = comps.get<Physics::Position>();
                const auto &col = comps.get<Physics::RectangularCollider>();

                if (player_pos.x + player_col.radius_x > pos.x - col.size_x)
                    player_velos.vx = player_velos.vx>0 ? 0 : player_velos.vx;

                if (player_pos.x - player_col.radius_x > pos.x + col.size_x)
                    player_velos.vx = player_velos.vx<0 ? 0 : player_velos.vx;

                if (player_pos.y + player_col.radius_y > pos.y - col.size_y)
                    player_velos.vy = player_velos.vy>0 ? 0 : player_velos.vy;

                if (player_pos.y - player_col.radius_y > pos.y + col.size_y)
                {
                    player_velos.vy = player_velos.vy<0 ? 0 : player_velos.vy;
                    player_c.on_ground  = true;
                }

                else
                    player_c.on_ground  = false;

            }
        }
    }
} // namespace Game::Overview
