#pragma once

namespace Game::World
{
    template<typename T>
    void detect_overlap_interactable([[maybe_unused]] T &syscall,
                        System::ECS::Query<GlobalState> &global_query,
                        System::ECS::Query<Input> &input_query,
                         System::ECS::Query<Interactable, EventState, Render::Transform, Physics::CircularCollider> &interactable_query,
                         System::ECS::Query<Player,Render::Transform, Physics::CircularCollider> &player_query)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &global_state = global_query.front().components.get<GlobalState>();
        const auto &input = input_query.front().components.get<Input>();

        for (auto &[pid, player_comps] : player_query)
        {
            const auto &player_pos = player_comps.get<Render::Transform>().position;

            for (auto &[id, comps] : interactable_query)
            {
                comps.get<Interactable>().in_range = false;
                const float dx = comps.get<Render::Transform>().position.x - player_pos.x;
                const float dy = comps.get<Render::Transform>().position.y - player_pos.y;
                const float distance_squared = dx * dx + dy * dy;

                const float trigger_radius = comps.get<Physics::CircularCollider>().radius_x * comps.get<Render::Transform>().scaleX;

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
                         System::ECS::Query<Block, Render::Transform, Physics::RectangularCollider> &block_query,
                         System::ECS::Query<Player,Render::Transform, Velocity, Physics::CircularCollider> &player_query)
    {
    constexpr float eps = 0.001f;
        constexpr float ground_snap = 0.05f;

    for (auto &[pid, player_comps] : player_query)
    {
        auto &player_tr = player_comps.get<Render::Transform>();
        auto &player_c = player_comps.get<Player>();
        auto &player_vel = player_comps.get<Velocity>();
        const auto &player_col = player_comps.get<Physics::CircularCollider>();
        auto &player_pos = player_tr.position;

        bool grounded = false;

        float player_half_x = player_col.radius_x * player_tr.scaleX;
        float player_half_y = player_col.radius_y * player_tr.scaleY;

        for (auto &[id, comps] : block_query)
        {
            const auto &tr = comps.get<Render::Transform>();
            const auto &col = comps.get<Physics::RectangularCollider>();
            const auto &pos = tr.position;

            float block_half_x = col.size_x * tr.scaleX;
            float block_half_y = col.size_y * tr.scaleY;

            bool overlap_x = player_pos.x + player_half_x > pos.x - block_half_x && player_pos.x - player_half_x < pos.x + block_half_x;
            bool overlap_y = player_pos.y + player_half_y > pos.y - block_half_y && player_pos.y - player_half_y < pos.y + block_half_y;

            if (!(overlap_x && overlap_y)) continue;

            float dx = player_pos.x - pos.x;
            float dy = player_pos.y - pos.y;

            float px = (block_half_x + player_half_x) - std::abs(dx);
            float py = (block_half_y + player_half_y) - std::abs(dy);

            if (px <= eps || py <= eps) continue;

            if (px < py)
            {
                if (dx > 0.0f)
                    player_pos.x += px;
                else
                    player_pos.x -= px;

                player_vel.vx = 0.0f;
            }
            else
            {
                if (dy > 0.0f)
                {
                    player_pos.y += py - ground_snap;
                    grounded = true;
                }
                else
                {
                    player_pos.y -= py;
                }
                player_vel.vy = 0.0f;
            }
        }
        player_c.on_ground = grounded;
    }
}
} // namespace Game::Overview
