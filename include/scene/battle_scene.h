#pragma once

#include "game.h"
#include "system.h"

namespace Scene
{
    struct BattleScene
    {
        static BattleScene instance();

        constexpr static auto name = "BattleScene";
        // declare scene parameters
        constexpr static size_t MaxResource = 1000;
        using ComponentTuple = std::tuple<
                Game::Battle::BattleState,
                Game::Battle::BulletHellState,
                Game::Battle::RhythmState,
                Game::Battle::ChartData,
                Game::Battle::LevelData,
                Game::BulletHell::Bullet,
                Game::BulletHell::Input,
                Game::BulletHell::Player,
                Game::BulletHell::Patterns,
                Game::BulletHell::BoomEmitter,
                Game::BulletHell::LaserEmitter,
                Game::BulletHell::RingEmitter,
                Game::BulletHell::RandomEmitter,
                Game::BulletHell::SpreadEmitter,
                Game::BulletHell::Bounce,
                Game::BulletHell::Homing,
                Game::BulletHell::Particle,
                Game::Physics::Acceleration,
                Game::Physics::CircularCollider,
                Game::Physics::RectangularCollider,
                Game::Physics::AngularVelocity,
                Game::Physics::Position,
                Game::Physics::Rotation,
                Game::Physics::Velocity,
                Game::Physics::Scale,
                Game::Rhythm::HoldActive,
                Game::Rhythm::KeyInput,
                Game::Rhythm::Lane,
                Game::Rhythm::NoteSpeed,
                Game::Rhythm::Timing,
                Game::Rhythm::TimingEnd,
                Game::Render::Animator,
                Game::Render::Rotation,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Text,
                Game::Render::Camera2D>;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<
                ResourceManager,
                Syscall,
                Game::Battle::input_system<Syscall>,
                Game::BulletHell::input_to_velocity<Syscall>,
                Game::BulletHell::movement_system<Syscall>,
                Game::BulletHell::rotation_system<Syscall>,
                Game::BulletHell::bullet_collision<Syscall>,
                Game::BulletHell::player_system<Syscall>,
                Game::BulletHell::bullet_system<Syscall>,
                Game::BulletHell::pattern_system<Syscall>,
                Game::BulletHell::bounce_pattern_system<Syscall>,
                Game::BulletHell::homing_pattern_system<Syscall>,
                Game::Rhythm::handle_rhythm<Syscall>,
                Game::Rhythm::handle_miss_note<Syscall>,
                Game::Rhythm::handle_bpm<Syscall>
                // Game::Render::AnimationSystem<Syscall>
                >;

        // declare functions
        static TaskManager init();
        static std::vector<ComponentTuple> exit();
    };
} // namespace Scene

/*
 * Explanation of System Dependency Graph (for future me)
 * INIT
 * LoadNotes: Load Notes TODO: Change from an UPDATE styled system to a utility function
 * UPDATE
 * IF Bullet Hell Side:
 * - InputToVelocity: Convert Input to Player Velocity
 * - MovementSystem: Movement System converts Velocity to Position
 * - ???: Query Collision into Player Damage/IFrame TODO: Implement logic
 * - PlayerSystem: Update IFrame
 * - BulletSystem: Update Bullet Telegraph/Damageable
 * ENDIF
 * IF Rhythm Game Side:
 * - HandleRhythm: Rhythm hit judgement calculations TODO: Implement logic
 * - HandleMissNote: Handle Miss Note
 * - HandleBPM: Handle BPM
 * ENDIF
 * System: Check Phase Change/Completion
 * IF Phase Change: Set phase of Battle State
 * IF Completion: Print Result
 */
