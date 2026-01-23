#pragma once

#include "system.h"
#include "game.h"

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
            Game::Rhythm::KeyInput,
            Game::Rhythm::Lane,
            Game::Rhythm::NoteSpeed,
            Game::Rhythm::Timing,
            Game::Rhythm::TimingEnd,
            Game::Render::Animator,
            Game::Render::Rotation,
            Game::Render::Sprite
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::Battle::InputSystem<Syscall>,
            Game::BulletHell::InputToVelocity<Syscall>,
            Game::BulletHell::MovementSystem<Syscall>,
            Game::BulletHell::RotationSystem<Syscall>,
            Game::BulletHell::SyncRotationSystem<Syscall>,
            Game::BulletHell::BulletCollision<Syscall>,
            Game::BulletHell::PlayerSystem<Syscall>,
            Game::BulletHell::BulletSystem<Syscall>,
            Game::BulletHell::PatternSystem<Syscall>,
            Game::BulletHell::BouncePatternSystem<Syscall>,
            //Game::BulletHell::HomingPatternSystem<Syscall>,
            Game::Rhythm::HandleRhythm<Syscall>,
            Game::Rhythm::HandleMissNote<Syscall>,
            Game::Rhythm::HandleBPM<Syscall> ,
            Game::Render::AnimationSystem<Syscall> >;

        // declare functions
        static TaskManager Init();
        static std::vector<ComponentTuple> Exit();
    };
}

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