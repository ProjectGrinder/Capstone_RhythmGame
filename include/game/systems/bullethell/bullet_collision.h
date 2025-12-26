#pragma once

#include "game/components.h"

namespace Game::BulletHell
{
	template<typename T>
	void BulletCollision([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState> &battle_query,
						 System::ECS::Query<Bullet,Physics::Position, Physics::Rotation, Physics::RectangularCollider> &bullet_query,
						 System::ECS::Query<Player,Physics::Position, Physics::CircularCollider> &player_query)
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

		for (auto &[id, comps] : bullet_query)
		{
			auto &bullet = comps.get<Bullet>();
		    const auto &bullet_pos = comps.get<Physics::Position>();
		    const auto &player_pos = player_query.front().get<Physics::Position>();
            const auto &bullet_hitbox = comps.get<Physics::RectangularCollider>();
            const auto &player_hitbox = player_query.front().get<Physics::CircularCollider>();

		    const float bullet_angle = comps.get<Physics::Rotation>().angle * std::acos(0.0f)/90.0f  ;

			if (!bullet.is_active)
                continue;

			// TODO: improve collision detection

            //Wide check : Acceptance dis
		    const float dx = bullet_pos.x - player_pos.x;
		    const float dy = bullet_pos.y - player_pos.y;
			const float distance_squared = dx * dx + dy * dy;
            if (constexpr float collision_distance_squared = player_hitbox.radius + std::max(bullet_hitbox.size.x, bullet_hitbox.size.y);
                distance_squared > collision_distance_squared || !bullet.is_damageable) continue;

		    //Narrow check : SAT
            const auto player_hitbox_pos = Position(player_pos.x + player_hitbox.offset.x, player_pos.y + player_hitbox.offset.y);
		    const auto bullet_hitbox_pos = Position(bullet_pos.x + bullet_hitbox.offset.x, bullet_pos.y + bullet_hitbox.offset.y);

		    const auto x_axis = Position(cos(bullet_angle),sin(bullet_angle));
		    const auto y_axis = Position(-sin(bullet_angle),cos(bullet_angle));
		    const auto distVec = Position(player_hitbox_pos.x - bullet_hitbox_pos.x, player_hitbox_pos.y - bullet_hitbox_pos.y);

		    const float dot_x = distVec.x * x_axis.x + distVec.y * x_axis.y;
		    const float dot_y = distVec.x * y_axis.x + distVec.y * y_axis.y;

		    if (fabs(dot_x) > bullet_hitbox.size.x/2 + player_hitbox.radius) continue;
		    if (fabs(dot_y) > bullet_hitbox.size.y/2 + player_hitbox.radius) continue;

		    const float distX = std::max(fabs(dot_x) - bullet_hitbox.size.x/2,.0f);
		    const float distY = std::max(fabs(dot_y) - bullet_hitbox.size.y/2,.0f);
		    if (distX * distX + distY * distY <= player_hitbox.radius * player_hitbox.radius) continue;

		    // Reduce player HP
		    auto &battle_state = battle_query.front().get<Battle::BattleState>();
		    battle_state.hp -= bullet.damage;
		    if (battle_state.hp < 0)
		        battle_state.hp = 0;

		    // Deactivate the bullet
		    bullet.is_active = false;

		    // Queue bullet for removal
		    syscall.remove_entity(id);

        }
	}
} // namespace Game::BulletHell
