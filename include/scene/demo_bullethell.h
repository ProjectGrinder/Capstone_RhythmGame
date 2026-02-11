#pragma once
#include "system.h"
#include "game.h"

namespace Scene
{
    inline void initBulletGraphic(std::array<Game::Battle::BulletGraphicMap, 128>& map)
    {
        using namespace Game::Battle;
        using namespace Game::Physics;
        map = {
            BulletGraphicMap(ColliderData(CIRCLE, 4,4)),
            BulletGraphicMap(ColliderData(CIRCLE, 8,8)),
            BulletGraphicMap(ColliderData(CIRCLE, 8,8), {}, {}, 1, 999, 3),
            BulletGraphicMap(ColliderData(CIRCLE, 8,8), {} , SpecialBulletData(Booming, 8, 3), 1, 999, 20),
            BulletGraphicMap(ColliderData(RECTANGLE, 8,8), {} , SpecialBulletData(Laser, 8, 3), 1, 999, 20),
        };
    }
    inline Game::Battle::BulletLoader create_bullet_data()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;
        using Game::BulletHell::Patterns;
        initBulletGraphic(bulletGraphicMap);

        BulletLoader loader;
        loader.CreateBullet(1000, BulletData(5, 5, Patterns(0,1, 135), 100, 0));
        loader.CreateBullet(1000, BulletData(5, 5, Patterns(0,0.9f, 120), 100, 0));
        loader.CreateBullet(1000, BulletData(5, 5, Patterns(0,0.8f, 105), 100, 0));
        loader.CreateBullet(1000, BulletData(5, 5, Patterns(0,0.9f, 150), 100, 0));
        loader.CreateBullet(1000, BulletData(5, 5, Patterns(0, 0.8f, 165), 100, 0));

        auto patterns1 = Patterns(0,1, 135);
        patterns1.AddPattern(500,1,45,0.1f,0, 2);
        loader.CreateBullet(2000, BulletData(10, 5, patterns1, 0, 1));
        loader.CreateBullet(2200, BulletData(20, 5, patterns1, 0, 1));
        loader.CreateBullet(2400, BulletData(30, 5, patterns1, 0, 1));
        loader.CreateBullet(2600, BulletData(40, 5, patterns1, 0, 1));
        loader.CreateBullet(2800, BulletData(50, 5, patterns1, 0, 1));

        loader.CreateBullet(4000, BulletData(25, 25, {}, 0, 3));
        loader.CreateBullet(4250, BulletData(50, 50, {}, 0, 3));
        loader.CreateBullet(4500, BulletData(75, 75, {}, 0, 3));

        loader.CreateBullet(5000, BulletData(150, 25, {}, 0, 3));
        loader.CreateBullet(5250, BulletData(125, 50, {}, 0, 3));
        loader.CreateBullet(5500, BulletData(100, 75, {}, 0, 3));

        loader.CreateBullet(6000, BulletData(25, 25, {}, 150, 4));
        loader.CreateBullet(6250, BulletData(50, 50, {}, 150, 4));
        loader.CreateBullet(6500, BulletData(75, 75, {}, 150, 4));

        loader.CreateBullet(7000, BulletData(150, 25, {}, 150, 4));
        loader.CreateBullet(7250, BulletData(125, 50, {}, 150, 4));
        loader.CreateBullet(7500, BulletData(100, 75, {}, 150, 4));
        return loader;
    }

    struct DemoBulletHell
    {
        static DemoBulletHell instance();

        constexpr static auto name = "DemoBulletHell";
        constexpr static size_t MaxResource = 1000;
        // declare scene parameters
        using ComponentTuple = std::tuple<
            Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletLoader,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData,
            Game::BulletHell::Bullet,
            Game::BulletHell::Input,
            Game::Rhythm::KeyInput,
            Game::BulletHell::Player,
            Game::BulletHell::Patterns,
            Game::BulletHell::Bounce,
            Game::BulletHell::Homing,
            Game::BulletHell::Particle,
            Game::BulletHell::Delay,
            Game::BulletHell::Booming,
            Game::BulletHell::Laser,
            Game::Physics::Acceleration,
            Game::Physics::CircularCollider,
            Game::Physics::RectangularCollider,
            Game::Physics::AngularVelocity,
            Game::Physics::Position,
            Game::Physics::Rotation,
            Game::Physics::Velocity,
            Game::Physics::Scale,
            Game::Render::Rotation,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::BulletHell::LoadBullets<Syscall>,
            Game::Battle::InputSystem<Syscall>,
            Game::BulletHell::InputToVelocity<Syscall>,
            Game::BulletHell::PatternSystem<Syscall>,
            Game::BulletHell::MovementSystem<Syscall>,
            Game::BulletHell::AccelerationSystem<Syscall>,
            Game::BulletHell::RotationSystem<Syscall>,
            Game::BulletHell::BulletCollision<Syscall>,
            Game::BulletHell::PlayerSystem<Syscall>,
            Game::BulletHell::DelaySystem<Syscall>,
            Game::BulletHell::ShotDelaySystem<Syscall>,
            Game::BulletHell::BulletSystem<Syscall>,
            Game::BulletHell::ParticleSystem<Syscall>,
            Game::BulletHell::Boomer_System<Syscall>,
            Game::BulletHell::Laser_System<Syscall>,
            Game::BulletHell::BouncePatternSystem<Syscall>,
            Game::BulletHell::HomingPatternSystem<Syscall>,
            Game::BulletHell::LoggingSystem<Syscall>
            >;

        static TaskManager Init()
        {
            auto tm = TaskManager{};
            // Create and configure BattleState
            tm.create_entity<Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletLoader,
            Game::Rhythm::KeyInput, Game::BulletHell::Input>
            (
                Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
                Game::Battle::BulletHellState(),
                create_bullet_data(),
                Game::Rhythm::KeyInput(),
                Game::BulletHell::Input()
                );

            tm.create_entity<Game::BulletHell::Player,
            Position, Rotation,
            Game::Physics::Scale,
            Velocity,
            Acceleration,
            AngularVelocity, Game::Physics::CircularCollider>(
                {}, {}, {}, {}, {}, {}, {}, {}
            );
            tm.run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> Exit();
    };
}