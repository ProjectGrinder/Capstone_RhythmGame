#pragma once
#include "game/components.h"

// TODO: Make this 💀
namespace Game::BulletHell
{
    using Position = Physics::Position;
    using Velocity = Physics::Velocity;
    using Rotation = Physics::Rotation;
    using Acceleration = Physics::Acceleration;
    using AngularVelocity = Physics::AngularVelocity;

    template <typename T>
    void BoomEmitterSystem([[maybe_unused]] T &syscall, System::ECS::Query<BoomEmitter>& query, System::ECS::Query<Battle::BattleState> &query2)
    {

    }
}