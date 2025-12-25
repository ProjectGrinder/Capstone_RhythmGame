#pragma once

#include "game/components.h"


namespace Game::BulletHell
{
	template<typename T>
	void BulletCollision([[maybe_unused]] T &syscall, System::ECS::Query<Battle::BattleState> &battle_query,
						 System::ECS::Query<Bullet, Physics::Position> &bullet_query,
						 System::ECS::Query<Player, Physics::Position> &player_query)
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

		// TODO: Suchas verify and reimplement the following system if desired

		for (auto &[id, comps] : bullet_query)
		{
			auto &bullet = comps.get<Bullet>();
            const auto &bullet_pos = comps.get<Physics::Position>();
            const auto &player_pos = player_query.front().get<Physics::Position>();

			if (!bullet.is_active)
                continue;

			// TODO: improve collision detection
			// Distance-based collision detection (Maybe this can be subbed with a collider when physics gets split off)
			const float dx = bullet_pos.x - player_pos.x;
			const float dy = bullet_pos.y - player_pos.y;
			const float distance_squared = dx * dx + dy * dy;
            if (constexpr float collision_distance_squared = 1.0f;
                distance_squared <= collision_distance_squared && bullet.is_damageable)
			{
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
	}
} // namespace Game::BulletHell
