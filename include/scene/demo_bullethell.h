#pragma once
#include "game.h"

namespace Scene
{
    inline Game::Battle::BulletRegistry init_bullet_graphic()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;
        std::vector<BulletGraphicMap> maps = {
            BulletGraphicMap(ColliderData(CIRCLE, 1,1)),
            BulletGraphicMap(ColliderData(CIRCLE, 1,1)),
            BulletGraphicMap(ColliderData(CIRCLE, 1,1), {}, {}, 20, 1, 100000),
            BulletGraphicMap(ColliderData(CIRCLE, 2,2), {} , SpecialBulletData(Booming, 5, 100), 5, 999, 3000),
            BulletGraphicMap(ColliderData(RECTANGLE, 2,2), {} , SpecialBulletData(Laser, 5, 100), 5, 999, 3000),
        };
        return {BulletRegistry(maps)};
    }

    inline Game::Battle::PatternContainer create_pattern_container()
    {
        using namespace Game::Battle;
        const std::vector<PatternStep> demo_step = {
            PatternStep(3000, OP_ADD, 2, 15),  // 3s Rot+15
            PatternStep(3000, OP_ADD, 2, -15), // 3s Rot-15
            PatternStep(3000, OP_SET, 1, 3),   // 3s Vel=3
    };
        const std::vector<PatternSequence> demo_pattern = {
            PatternSequence(false),
            PatternSequence(false, 0, 2),
            PatternSequence(false, 1, 2),
            PatternSequence(true, 0, 2),
            PatternSequence(true, 1, 2),
        };
        auto demo_pattern_container = PatternContainer(demo_step,demo_pattern);
        return { PatternContainer(demo_pattern_container) };
    }

    inline Game::Battle::BulletLoader create_bullet_data()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;

        int left_padding = 42000;

        BulletLoader loader;
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 1,135, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 120 ,  100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.8f, 105, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5,0.9f, 150, 100, 0));
        loader.CreateBullet(left_padding + 1000, BulletData(5, 5, 0.8f, 165, 100, 0));

        loader.CreateBullet(left_padding + 3000, BulletData(10, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(20, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(40, 5, 1,135, 1, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(50, 5, 1,135, 1, 0, 1));

        loader.CreateBullet(left_padding + 3000, BulletData(50, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 3500, BulletData(40, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4000, BulletData(30, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 4500, BulletData(20, 5, 1,135, 2, 0, 1));
        loader.CreateBullet(left_padding + 5000, BulletData(10, 5, 1,135, 2, 0, 1));

        loader.CreateBullet(left_padding + 7000, BulletData(25, 25, 0, 3));
        loader.CreateBullet(left_padding + 7500, BulletData(50, 50, 0, 3));
        loader.CreateBullet(left_padding + 8000, BulletData(75, 75, 0, 3));

        loader.CreateBullet(left_padding + 10000, BulletData(150, 25, 0, 3));
        loader.CreateBullet(left_padding + 10500, BulletData(125, 50, 0, 3));
        loader.CreateBullet(left_padding + 11000, BulletData(100, 75, 0, 3));

        loader.CreateBullet(left_padding + 12000, BulletData(25, 25, 150, 4));
        loader.CreateBullet(left_padding + 12500, BulletData(50, 50, 150, 4));
        loader.CreateBullet(left_padding + 13000, BulletData(75, 75, 150, 4));

        loader.CreateBullet(left_padding + 14000, BulletData(150, 25, 150, 4));
        loader.CreateBullet(left_padding + 14500, BulletData(125, 50, 150, 4));
        loader.CreateBullet(left_padding + 15000, BulletData(100, 75, 150, 4));
        return loader;
    }

    inline Game::Battle::BulletLoader create_bullet_data2()
    {
        using namespace Game::Battle;
        using namespace Game::Physics;

        BulletLoader loader;
        loader.CreateBullet(5000, BulletData(0, 0, 0,0, 1000, 3));
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
            Game::Battle::BulletRegistry,
            Game::Battle::BulletLoader,
            Game::Battle::PatternContainer,
            Game::Battle::RhythmState,
            Game::Battle::ChartData,
            Game::Battle::LevelData,
            Game::Battle::TransitionData,
            Game::BulletHell::Bullet,
            Game::BulletHell::BulletClearer,
            Game::BulletHell::Input,
            Game::Rhythm::KeyInput,
            Game::BulletHell::Player,
            Game::BulletHell::Pattern,
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
            Game::Render::Transform,
            Game::Render::Sprite,
            Game::Render::Material,
            Game::Render::Text,
            Game::Render::Camera2D,
            Game::Rhythm::NoteType
            >;
        using ResourceManager = Utils::make_resource_manager_t<MaxResource, ComponentTuple>;
        using Syscall = Utils::make_syscall_t<MaxResource, ComponentTuple>;
        using TaskManager = System::ECS::TaskManager<ResourceManager, Syscall,
            Game::BulletHell::load_bullets<Syscall>,
            Game::Battle::input_system<Syscall>,
            Game::BulletHell::input_to_velocity<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::movement_system<Syscall>,
            Game::BulletHell::acceleration_system<Syscall>,
            Game::BulletHell::rotation_system<Syscall>,
            Game::BulletHell::bullet_collision<Syscall>,
            Game::BulletHell::bullet_clearer_system<Syscall>,
            Game::BulletHell::player_system<Syscall>,
            Game::BulletHell::delay_system<Syscall>,
            Game::BulletHell::bullet_system<Syscall>,
            Game::BulletHell::particle_system<Syscall>,
            Game::BulletHell::boomer_system<Syscall>,
            Game::BulletHell::laser_system<Syscall>,
            Game::BulletHell::bounce_pattern_system<Syscall>,
            Game::BulletHell::homing_pattern_system<Syscall>,
            Game::BulletHell::logging_system<Syscall>,
            Game::Battle::update_global_clock<Syscall>
            >;

        static std::shared_ptr<TaskManager> init()
        {
            auto tm = std::make_shared<TaskManager>();
            // Create and configure BattleState
            tm->create_entity<Game::Battle::BattleState,
            Game::Battle::BulletHellState,
            Game::Battle::BulletRegistry,
            Game::Battle::BulletLoader,
            Game::Battle::PatternContainer,
            Game::Rhythm::KeyInput, Game::BulletHell::Input>
            (
                Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
                Game::Battle::BulletHellState(),
                init_bullet_graphic(),
                create_bullet_data2(),
                create_pattern_container(),
                Game::Rhythm::KeyInput(),
                Game::BulletHell::Input()
                );

            tm->create_entity<Game::BulletHell::Player,
            Position, Rotation,
            Game::Physics::Scale,
            Velocity,
            Acceleration,
            AngularVelocity, Game::Physics::CircularCollider>(
                {}, {}, {}, {}, {}, {}, {}, {}
            );
            tm->run_all();
            return (tm);
        }

        static std::vector<ComponentTuple> exit(std::shared_ptr<TaskManager> &manager);
    };
}