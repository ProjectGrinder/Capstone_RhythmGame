#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
    // No layer/mask implemented yet
	template<typename T>
	void BulletCollision([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState> &battle_query,
	                     System::ECS::Query<Battle::BulletHellState> &state_query,
						 System::ECS::Query<Bullet,Physics::Position, Physics::Rotation, Physics::Scale, Physics::RectangularCollider> &bullet_query1,
						 System::ECS::Query<Bullet,Physics::Position, Physics::Rotation, Physics::Scale, Physics::CircularCollider> &bullet_query2,
						 System::ECS::Query<Player,Physics::Position, Physics::Scale, Physics::CircularCollider> &player_query)
	{
		if (battle_query.begin() == battle_query.end())
			return;

		if (battle_query.front().get<Battle::BattleState>().current_phase != Battle::CurrentPhase::BULLET_HELL)
		{
			return;
		}

		if (player_query.begin() == player_query.end())
            return;

		if (player_query.front().get<Player>().is_active == false)
            return;

	    const auto &player_pos = player_query.front().get<Physics::Position>();
	    const auto &player_hitbox = player_query.front().get<Physics::CircularCollider>();
	    const auto &player_scl = player_query.front().get<Physics::Scale>();
	    const float player_hitbox_size = player_hitbox.radius_x * (player_scl.scaleX < player_scl.scaleY ? player_scl.scaleX : player_scl.scaleY);
	    auto &state = state_query.front().get<Battle::BattleState>();
		for (auto &[id, comps] : bullet_query1)
		{
            if (state.iframe_time > 0)
                break;

			auto &bullet = comps.get<Bullet>();
		    const auto &bullet_pos = comps.get<Physics::Position>();
            const auto &bullet_hitbox = comps.get<Physics::RectangularCollider>();
		    const auto &bullet_scl = comps.get<Physics::Scale>();

		    const float bullet_angle = comps.get<Physics::Rotation>().angleZ * std::acos(0.0f)/90.0f;
		    const float bullet_hitbox_size_x = bullet_hitbox.size_x * bullet_scl.scaleX;
		    const float bullet_hitbox_size_y = bullet_hitbox.size_y * bullet_scl.scaleY;

			if (!bullet.is_active)
                continue;

			// TODO: improve collision detection
            //Wide check
		    const float dx = bullet_pos.x - player_pos.x;
		    const float dy = bullet_pos.y - player_pos.y;
			const float distance_squared = dx * dx + dy * dy;
		    const float collision_distance = player_hitbox_size + ((bullet_hitbox_size_x > bullet_hitbox_size_y)
	                                                            ? bullet_hitbox_size_x
                                                                : bullet_hitbox_size_y) * 0.5f;
            if (distance_squared > collision_distance * collision_distance || !bullet.is_damageable) continue;
		    //Narrow check : SAT
            const auto player_hitbox_pos = Position(player_pos.x + player_hitbox.offset_x, player_pos.y + player_hitbox.offset_y);
		    const auto bullet_hitbox_pos = Position(bullet_pos.x + bullet_hitbox.offset_x, bullet_pos.y + bullet_hitbox.offset_y);

		    const auto distVec = Position(player_hitbox_pos.x - bullet_hitbox_pos.x, player_hitbox_pos.y - bullet_hitbox_pos.y);

		    const float dot_x = distVec.x * cos(bullet_angle) + distVec.y * sin(bullet_angle);
		    const float dot_y = distVec.x * -sin(bullet_angle) + distVec.y * cos(bullet_angle);

		    const float distX =
                    (((fabs(dot_x) - bullet_hitbox_size_x / 2) > (.0f)) ? (fabs(dot_x) - bullet_hitbox_size_x / 2)
                                                                        : (.0f));
            const float distY =
                    (((fabs(dot_y) - bullet_hitbox_size_y / 2) > (.0f)) ? (fabs(dot_y) - bullet_hitbox_size_y / 2)
                                                                        : (.0f));
		    if (distX * distX + distY * distY >= player_hitbox_size * player_hitbox_size) continue;

		    // Reduce player HP
		    auto &battle_state = battle_query.front().get<Battle::BattleState>();
		    battle_state.hp -= bullet.damage;
		    if (battle_state.hp < 0)
		        battle_state.hp = 0;

		    // TODO : Make this const
		    // Activate Player iFrame
		    state.iframe_time = 120;

		    // Deactivate the bullet
		    bullet.pierce --;

        }

	    for (auto &[id, comps] : bullet_query2)
	    {
	        if (state.iframe_time > 0)
	            break;

	        auto &bullet = comps.get<Bullet>();
	        const auto &bullet_pos = comps.get<Physics::Position>();
	        const auto &bullet_hitbox = comps.get<Physics::CircularCollider>();
	        const auto &bullet_scl = comps.get<Physics::Scale>();

	        const float bullet_angle = comps.get<Physics::Rotation>().angleZ * std::acos(0.0f)/90.0f;
	        const float bullet_hitbox_size_x = bullet_hitbox.radius_x * bullet_scl.scaleX;
	        const float bullet_hitbox_size_y = bullet_hitbox.radius_y * bullet_scl.scaleY;
	        if (!bullet.is_active)
	            continue;

	        //Wide check
	        const float dx = bullet_pos.x - player_pos.x;
	        const float dy = bullet_pos.y - player_pos.y;
	        const float distance_squared = dx * dx + dy * dy;
	        const float collision_distance = player_hitbox_size + ((bullet_hitbox_size_x > bullet_hitbox_size_y)
	                                                            ? bullet_hitbox_size_x
                                                                : bullet_hitbox_size_y);
	        if (distance_squared > collision_distance*collision_distance || !bullet.is_damageable)
	            continue;

	        // Ellipse vs circle collision
	        const auto player_hitbox_pos = Position(player_pos.x + player_hitbox.offset_x, player_pos.y + player_hitbox.offset_y);
	        const auto bullet_hitbox_pos =
                    Position(bullet_pos.x + bullet_hitbox.offset_x, bullet_pos.y + bullet_hitbox.offset_y);
            const float dist_x = player_hitbox_pos.x - bullet_hitbox_pos.x;
            const float dist_y = player_hitbox_pos.y - bullet_hitbox_pos.y;

	        float rDist_x = dist_x * cos(-bullet_angle) - dist_y * sin(-bullet_angle);
	        float rDist_y = dist_x * sin(-bullet_angle) + dist_y * cos(-bullet_angle);

	        rDist_x /= bullet_hitbox_size_x;
	        rDist_y /= bullet_hitbox_size_y;
	        float rScaled = player_hitbox_size / (bullet_hitbox_size_x < bullet_hitbox_size_y ? bullet_hitbox_size_x: bullet_hitbox_size_y);

	        if (rDist_x * rDist_x + rDist_y * rDist_y >= (1.0f + rScaled) * (1.0f + rScaled)) continue;


	        // Reduce player HP
	        auto &battle_state = battle_query.front().get<Battle::BattleState>();
	        battle_state.hp -= bullet.damage;
	        if (battle_state.hp < 0)
	            battle_state.hp = 0;

	        // TODO : Make this const
	        // Activate Player iFrame
	        state.iframe_time = 120;

	        // Deactivate the bullet
	        bullet.pierce--;
	    }
	}
} // namespace Game::BulletHell
