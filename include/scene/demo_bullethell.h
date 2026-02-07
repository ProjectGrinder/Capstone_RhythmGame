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
        loader.CreateBullet(30, BulletData(5, 5, Patterns(0,1, 135), 10, 0));
        loader.CreateBullet(30, BulletData(5, 5, Patterns(0,0.9f, 120), 10, 0));
        loader.CreateBullet(30, BulletData(5, 5, Patterns(0,0.8f, 105), 10, 0));
        loader.CreateBullet(30, BulletData(5, 5, Patterns(0,0.9f, 150), 10, 0));
        loader.CreateBullet(30, BulletData(5, 5, Patterns(0, 0.8f, 165), 10, 0));

        auto patterns1 = Patterns(0,1, 135);
        patterns1.AddPattern(30,1,45,0.1f,0, 2);
        loader.CreateBullet(60, BulletData(10, 5, patterns1, 0, 1));
        loader.CreateBullet(70, BulletData(20, 5, patterns1, 0, 1));
        loader.CreateBullet(80, BulletData(30, 5, patterns1, 0, 1));
        loader.CreateBullet(90, BulletData(40, 5, patterns1, 0, 1));
        loader.CreateBullet(100, BulletData(50, 5, patterns1, 0, 1));

        loader.CreateBullet(120, BulletData(25, 25, {}, 0, 3));
        loader.CreateBullet(150, BulletData(50, 50, {}, 0, 3));
        loader.CreateBullet(180, BulletData(75, 75, {}, 0, 3));

        loader.CreateBullet(240, BulletData(150, 25, {}, 0, 3));
        loader.CreateBullet(270, BulletData(125, 50, {}, 0, 3));
        loader.CreateBullet(300, BulletData(100, 75, {}, 0, 3));

        loader.CreateBullet(330, BulletData(25, 25, {}, 15, 4));
        loader.CreateBullet(360, BulletData(50, 50, {}, 15, 4));
        loader.CreateBullet(390, BulletData(75, 75, {}, 15, 4));

        loader.CreateBullet(420, BulletData(150, 25, {}, 15, 4));
        loader.CreateBullet(450, BulletData(125, 50, {}, 15, 4));
        loader.CreateBullet(480, BulletData(100, 75, {}, 15, 4));
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
            Game::BulletHell::HomingPatternSystem<Syscall>
            >;

        static void test();

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


            tm.run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> Exit();
    };
}