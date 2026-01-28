#pragma once
#include "game/components.h"

// TODO: Make this 💀
namespace Game::BulletHell
{
    template <typename T>
    void BoomEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<BoomEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        // Idk : 1. Create More Comp -> Emitter obsolete , 2. Alternate Workaround -> Bad practice
    }

    template <typename T>
    void LaserEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<LaserEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        // Idk : 1. Create More Comp -> Emitter obsolete , 2. Alternate Workaround -> Bad practice
    }

    template <typename T>
    void RingEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<RingEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        // TODO : Using Particle Prefab
        //  1. Assign MoveRotation = emitter.rotation
        //  2. For each particle
        //      2.1 calculate position of particle relative to width & height & bullet_quantity
        //      2.2 assign moveRot = rotation + bullet_fired*360/bullet_quantity
        //      2.2 bullet_fired++
        //      2.3 if bullet_fired = quantity -> destroy
        //      2.4 add delay for fire_delay
    }

    template <typename T>
    void RandomEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<RingEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        // TODO : Using Particle Prefab
        //  1. Assign MoveRotation = emitter.rotation
        //  2. For each particle
        //      2.1 random position of particle with width & height
        //      2.2 assign moveRot = center_rot + random from angle_range
        //      2.2 quantity--
        //      2.3 if quantity = 0 -> destroy
        //      2.4 add delay for fire_delay
    }

    template <typename T>
    void SpreadEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<RingEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        // TODO : Using Particle Prefab
        //  1. Assign MoveRotation = emitter.rotation
        //  2. For each particle
        //      2.1 calculate position of particle relative to width & height & bullet_quantity
        //      2.2 assign moveRot = rotation + smth
        //      2.2 bullet_fired++
        //      2.3 if bullet_fired = quantity -> destroy
        //      2.4 add delay for fire_delay
    }
}