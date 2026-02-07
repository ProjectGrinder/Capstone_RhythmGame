#include <utility>

#include "system/ecs.h"
#include "game.h"
#include "pch.h"

using System::ECS::pid;
using System::ECS::ResourcePool;
using System::ECS::SyscallType;
using System::ECS::TaskManager;
using Game::Battle::BattleState;
using Game::Battle::BulletHellState;
using namespace Game::BulletHell;
using namespace Game::Physics;

// -- BULLET HELL TESTS --

using BulletHell_Resource = System::ECS::ResourceManager<1000,Player, Bullet, Position, Scale, Velocity, Rotation, Acceleration, AngularVelocity, Booming, Delay, Laser, Particle, Patterns, CircularCollider, RectangularCollider, Game::Render::Material, BattleState, BulletHellState>;

using BulletHell_Syscall = System::ECS::Syscall<1000,Player, Bullet, Position, Scale, Velocity, Rotation, Acceleration, AngularVelocity, Booming, Delay, Laser, Particle, Patterns, CircularCollider, RectangularCollider, Game::Render::Material, BattleState, BulletHellState>;

pid CreatePlayer(BulletHell_Resource *resource, CircularCollider cc = {},
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0), Scale scl = {},
    Acceleration acc = Acceleration(0,0), AngularVelocity avel = AngularVelocity(0))
{
    const pid id = resource->reserve_process();
    resource->add_resource<Player>(id, Player());
    resource->add_resource<CircularCollider>(id, CircularCollider(cc));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Scale>(id, Scale(scl));
    resource->add_resource<Acceleration>(id,Acceleration(acc));
    resource->add_resource<AngularVelocity>(id,AngularVelocity(avel));
    return id;
}

pid CreateBullet1(BulletHell_Resource *resource, Bullet bullet = {},
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0),
    Acceleration acc = Acceleration(0,0), AngularVelocity avel = AngularVelocity(0))
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Acceleration>(id,Acceleration(acc));
    resource->add_resource<AngularVelocity>(id,AngularVelocity(avel));
    return id;
}

pid CreateBullet2(BulletHell_Resource *resource, Bullet bullet = {}, Delay delay = Delay(0),
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0),
    Acceleration acc = Acceleration(0,0), AngularVelocity avel = AngularVelocity(0))
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->add_resource<Delay>(id, Delay(delay));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Acceleration>(id,Acceleration(acc));
    resource->add_resource<AngularVelocity>(id,AngularVelocity(avel));
    return id;
}

pid CreateCircleBullet(BulletHell_Resource *resource, Bullet bullet = {}, CircularCollider cc = CircularCollider(),
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0), Scale scl = {})
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->add_resource<CircularCollider>(id, CircularCollider(cc));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Scale>(id, Scale(scl));
    return id;
}

pid CreateRectangleBullet(BulletHell_Resource *resource, Bullet bullet = {}, RectangularCollider rc = RectangularCollider(),
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0), Scale scl = {})
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->add_resource<RectangularCollider>(id, RectangularCollider(rc));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Scale>(id, Scale(scl));
    return id;
}


pid CreateBoomerBullet(BulletHell_Resource *resource, Bullet bullet = {}, Booming booming = {},Delay delay = Delay(0), Particle particle = {},
    Position pos = {}, CircularCollider cc = {})
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->query<Bullet>().get(id).pierce = 999;
    resource->add_resource<Delay>(id, Delay(delay));
    resource->add_resource<Particle>(id, Particle(particle));
    resource->add_resource<Booming>(id, Booming(booming));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Rotation>(id,{});
    resource->add_resource<Scale>(id, Scale());
    resource->add_resource<Game::Render::Material>(id,{});
    resource->add_resource<CircularCollider>(id, CircularCollider(cc));
    return id;
}

pid CreateLaserBullet(BulletHell_Resource *resource, Bullet bullet = {}, Laser laser = {},Delay delay = Delay(0), Particle particle = {},
    Rotation rot = {}, Scale scl = {}, RectangularCollider rc = {})
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->query<Bullet>().get(id).pierce = 999;
    resource->add_resource<Delay>(id, Delay(delay));
    resource->add_resource<Particle>(id, Particle(particle));
    resource->add_resource<Laser>(id, Laser(laser));
    resource->add_resource<Position>(id, Position());
    resource->add_resource<Rotation>(id,Rotation(rot));
    resource->add_resource<Scale>(id, Scale(scl));
    resource->add_resource<Game::Render::Material>(id,{});
    resource->add_resource<RectangularCollider>(id, RectangularCollider(rc));
    return id;
}

void AddPattern(BulletHell_Resource *resource,
        const pid bullet_id,
        const float delay,
        const float angle = UNASSIGNED,
        const float speed = UNASSIGNED,
        const float acceleration = UNASSIGNED,
        const float angular_velocity = UNASSIGNED,
        const float max_speed = UNASSIGNED,
        const float loopDelay = UNASSIGNED)
{
    if (!resource->query<Patterns>().has(bullet_id)) resource->add_resource<Patterns>(bullet_id, Patterns());
    auto &patterns = resource->query<Patterns>().get(bullet_id);
    patterns.AddPattern(delay, speed, angle, acceleration, angular_velocity,max_speed,loopDelay);
}

pid CreateBattleState(BulletHell_Resource *resource)
{
    const pid id = resource->reserve_process();
    resource->add_resource<BattleState>(id, BattleState());
    return id;
}

pid CreateBulletHellState(BulletHell_Resource *resource)
{
    const pid id = resource->reserve_process();
    resource->add_resource<BulletHellState>(id, BulletHellState());
    return id;
}

// Test Entity Instantiation

TEST(Game, instantiate_player)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreatePlayer(resource);
    EXPECT_TRUE(resource->query<Player>().get(id).is_active);
    EXPECT_TRUE(resource->query<Position>().has(id));
    EXPECT_TRUE(resource->query<Velocity>().has(id));
    EXPECT_TRUE(resource->query<Rotation>().has(id));
    EXPECT_TRUE(resource->query<Acceleration>().has(id));
    EXPECT_TRUE(resource->query<AngularVelocity>().has(id));
    EXPECT_TRUE(resource->query<Player>().has(id));
}



TEST(Game, instantiate_bullets)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id_1 = CreateBullet1(resource);

    EXPECT_TRUE(resource->query<Position>().has(id_1));
    EXPECT_TRUE(resource->query<Velocity>().has(id_1));
    EXPECT_TRUE(resource->query<Bullet>().has(id_1));
    EXPECT_EQ(resource->query<Bullet>().get(id_1).damage, 0);

    const pid id_2 = CreateBullet1(resource);
    const pid id_3 = CreateBullet1(resource);

    EXPECT_TRUE(resource->query<Position>().has(id_2));
    EXPECT_TRUE(resource->query<Velocity>().has(id_2));
    EXPECT_TRUE(resource->query<Bullet>().has(id_2));
    EXPECT_TRUE(resource->query<Position>().has(id_3));
    EXPECT_TRUE(resource->query<Velocity>().has(id_3));
    EXPECT_TRUE(resource->query<Bullet>().has(id_3));
    EXPECT_EQ(resource->query<Bullet>().get(id_2).damage, 0);
    EXPECT_EQ(resource->query<Bullet>().get(id_3).damage, 0);
}

TEST(Game, value_change)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id_1 = CreateBullet1(resource);
    const pid id_2 = CreateBullet1(resource);

    EXPECT_TRUE(resource->query<Bullet>().has(id_1));
    EXPECT_TRUE(resource->query<Bullet>().has(id_2));
    EXPECT_EQ(resource->query<Bullet>().get(id_1).damage, 0);
    EXPECT_EQ(resource->query<Bullet>().get(id_2).damage, 0);

    resource->query<Bullet>().get(id_1).damage = 1;

    EXPECT_TRUE(resource->query<Bullet>().has(id_1));
    EXPECT_TRUE(resource->query<Bullet>().has(id_2));
    EXPECT_EQ(resource->query<Bullet>().get(id_1).damage, 1);
    EXPECT_EQ(resource->query<Bullet>().get(id_2).damage, 0);
}

// TODO: Tests failed from here onwards with a compile error, trying to find cause of error

//Movement
TEST(Game, bullet_movement1)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet1(resource);
    resource->query<Position>().set(id,Position(1,1));
    resource->query<Velocity>().set(id,Velocity(2,3));

    EXPECT_EQ(resource->query<Position>().get(id).x, 1);
    EXPECT_EQ(resource->query<Position>().get(id).y, 1);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 3);

    //Move 1 time : 1,1 -> 3,4
    task_manager.run_all();

    EXPECT_EQ(resource->query<Position>().get(id).x, 3);
    EXPECT_EQ(resource->query<Position>().get(id).y, 4);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 3);

    //Move 2 times : 3,4 -> 7,10
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(id).x, 7);
    EXPECT_EQ(resource->query<Position>().get(id).y, 10);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 3);
}



TEST(Game, bullet_movement2)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet1(resource);
    resource->query<Position>().set(id,Position(0,0));
    resource->query<Velocity>().set(id,Velocity(4,0));
    resource->query<Rotation>().set(id,Rotation(30));

    //Object rotate 45 degree -> vx = 4*cos(angle) ; vy = 4*sin(angle)
    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(id).x), round(4*sqrt(3)/2));
    EXPECT_EQ(resource->query<Position>().get(id).y, 4*1/2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 4);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 0);
    EXPECT_EQ(resource->query<Rotation>().get(id).angleZ, 30);

    resource->query<Position>().set(id,Position(3,2));
    resource->query<Velocity>().set(id,Velocity(2,3));
    resource->query<Rotation>().set(id,Rotation(-90));

    //Object rotate -90 degree -> vx = vy ; vy = -vx
    task_manager.run_all();
    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(id).x), 9);
    EXPECT_EQ(round(resource->query<Position>().get(id).y), -2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 2);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 3);
    EXPECT_EQ(resource->query<Rotation>().get(id).angleZ, -90);
}

TEST(Game, bullet_movement3)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, AccelerationSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet1(resource);
    resource->query<Position>().set(id,Position(1,1));
    resource->query<Velocity>().set(id,Velocity(2,3));
    resource->query<Acceleration>().set(id,Acceleration(1,-1,999,-999));

    //Move 1 time : Pos 1,1 -> 3,4 then Vel 2,3 -> 3,2
    task_manager.run_all();

    EXPECT_EQ(resource->query<Position>().get(id).x, 3);
    EXPECT_EQ(resource->query<Position>().get(id).y, 4);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 3);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 2);

    //Move 1 times : 3,4 -> 6,7 then Vel 2,3 -> 4,1
    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(id).x, 6);
    EXPECT_EQ(resource->query<Position>().get(id).y, 6);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 4);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, 1);

    //Move 2 times : 6,6 -> 15,5 then Vel 4,1 -> 6,-1
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(id).x, 15);
    EXPECT_EQ(resource->query<Position>().get(id).y, 7);
    EXPECT_EQ(resource->query<Velocity>().get(id).vx, 6);
    EXPECT_EQ(resource->query<Velocity>().get(id).vy, -1);
}

TEST(Game, bullet_movement4)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, RotationSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet1(resource);
    resource->query<Rotation>().set(id,Rotation(30));
    resource->query<AngularVelocity>().set(id,AngularVelocity(-15));

    //Rotate 1 time : 30 -> 15
    task_manager.run_all();

    EXPECT_EQ(resource->query<Rotation>().get(id).angleZ, 15);
    EXPECT_EQ(resource->query<AngularVelocity>().get(id).v, -15);

    //Rotate 2 time : 15 -> -15
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Rotation>().get(id).angleZ, -15);
    EXPECT_EQ(resource->query<AngularVelocity>().get(id).v, -15);
}

TEST(Game, bullet_movement5)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, AccelerationSystem<BulletHell_Syscall>, RotationSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet1(resource);
    resource->query<Position>().set(id,Position(1,1));
    resource->query<Velocity>().set(id,Velocity(1,1));
    resource->query<Acceleration>().set(id,Acceleration(1,-1,999,-999));
    resource->query<Rotation>().set(id,Rotation(0));
    resource->query<AngularVelocity>().set(id,AngularVelocity(90));

    task_manager.run_all();

    EXPECT_EQ(resource->query<Position>().get(id).x, 2);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2);

    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Position>().get(id).x), 2);
    EXPECT_EQ(round(resource->query<Position>().get(id).y), 4);

    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Position>().get(id).x), -1);
    EXPECT_EQ(round(resource->query<Position>().get(id).y), 5);

    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Position>().get(id).x), -3);
    EXPECT_EQ(round(resource->query<Position>().get(id).y), 1);
}

TEST(Game, bullets_movement6)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, AccelerationSystem<BulletHell_Syscall>, RotationSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id_1 = CreateBullet1(resource,{},Position(1,1),Velocity(1,1),Rotation(0),Acceleration(1,-1,999,-999),AngularVelocity(90));
    const pid id_2 = CreateBullet1(resource,{},Position(2,0),Velocity(2,3),Rotation(30),{},AngularVelocity(30));
    const pid id_3 = CreateBullet1(resource, {},Position(-1,-3),Velocity(2,1),Rotation(180),Acceleration(-1,-2,-999,-999), {});

    EXPECT_EQ(resource->query<Position>().get(id_1).x, 1);
    EXPECT_EQ(resource->query<Position>().get(id_1).y, 1);
    EXPECT_EQ(resource->query<Position>().get(id_2).x, 2);
    EXPECT_EQ(resource->query<Position>().get(id_2).y, 0);
    EXPECT_EQ(resource->query<Position>().get(id_3).x, -1);
    EXPECT_EQ(resource->query<Position>().get(id_3).y, -3);
    task_manager.run_all();

    EXPECT_EQ(resource->query<Position>().get(id_1).x, 2);
    EXPECT_EQ(resource->query<Position>().get(id_1).y, 2);
    EXPECT_EQ(round(resource->query<Position>().get(id_2).x), 2);
    EXPECT_EQ(round(resource->query<Position>().get(id_2).y), 4);
    EXPECT_EQ(resource->query<Position>().get(id_3).x, -3);
    EXPECT_EQ(resource->query<Position>().get(id_3).y, -4);

    task_manager.run_all();
    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(id_1).x), -1);
    EXPECT_EQ(round(resource->query<Position>().get(id_1).y), 5);
    EXPECT_EQ(round(resource->query<Position>().get(id_2).x), -2);
    EXPECT_EQ(round(resource->query<Position>().get(id_2).y), 9);
    EXPECT_EQ(round(resource->query<Position>().get(id_3).x), -4);
    EXPECT_EQ(resource->query<Position>().get(id_3).y, 0);

}

TEST(Game, bullets_damagable)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, BulletSystem<BulletHell_Syscall>, DelaySystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBullet2(resource,Bullet(false,3),Delay(2));
    EXPECT_FALSE(resource->query<Bullet>().get(id).is_damageable);
    task_manager.run_all();
    EXPECT_FALSE(resource->query<Bullet>().get(id).is_damageable);
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 0);
    EXPECT_FALSE(resource->query<Bullet>().get(id).is_damageable);
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().get(id).is_damageable);
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().get(id).is_damageable);
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 0);
}

TEST(Game, bullets_booming)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, DelaySystem<BulletHell_Syscall>, ParticleSystem<BulletHell_Syscall>, Boomer_System<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBoomerBullet(resource, {}, Booming(3,2), Delay(3), Particle(10) );
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 2);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 3);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleY, 3);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 0.25);
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 1);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 0);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleY, 0);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 0);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 1.5f);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleY, 1.5f);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    EXPECT_EQ(resource->query<Particle>().get(id).lifetime, 7);
    task_manager.run_all();
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 1);
    EXPECT_GE(resource->query<Scale>().get(id).scaleX, 2.75f);
    EXPECT_GE(resource->query<Scale>().get(id).scaleY, 2.75f);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    EXPECT_EQ(resource->query<Particle>().get(id).lifetime, 4);
    task_manager.run_all();
    EXPECT_LE(resource->query<Scale>().get(id).scaleY, 2.75f);
    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Scale>().get(id).scaleX), 0);
}

TEST(Game, bullets_laser)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, DelaySystem<BulletHell_Syscall>, ParticleSystem<BulletHell_Syscall>, Laser_System<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateLaserBullet(resource, {}, Laser(2,2,3,3), Delay(3), Particle(10),Rotation(30) );
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 2);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 3);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleY, 1);
    EXPECT_EQ(round(resource->query<Position>().get(id).x * 100)/100.f, 3.3f);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2.75f);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 0.25);
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 1);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 0);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleY, 1);
    EXPECT_EQ(resource->query<Position>().get(id).x, 2);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 0);
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 1);
    EXPECT_EQ(round(resource->query<Position>().get(id).x * 100)/100.f, 2.43f);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2.25);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    EXPECT_EQ(resource->query<Particle>().get(id).lifetime, 7);
    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Position>().get(id).x * 100)/100.f, 2.87f);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2.5);
    task_manager.run_all();

    EXPECT_EQ(resource->query<Delay>().get(id).delay, 1);
    EXPECT_GE(resource->query<Scale>().get(id).scaleX, 3);
    EXPECT_EQ(round(resource->query<Position>().get(id).x * 100)/100.f, 3.3f);
    EXPECT_EQ(resource->query<Position>().get(id).y, 2.75f);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    EXPECT_EQ(resource->query<Particle>().get(id).lifetime, 5);
    task_manager.run_all();
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Scale>().get(id).scaleX, 0);
    EXPECT_EQ(round(resource->query<Position>().get(id).x * 100)/100.f, 5.9f);
    EXPECT_EQ(resource->query<Position>().get(id).y, 4.25);
}

// Collision
TEST(Game, bullet_collision1)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, BulletCollision<BulletHell_Syscall>, BulletSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);
    const pid bhs = CreateBulletHellState(resource);

    CreatePlayer(resource, CircularCollider(2));
    const pid bullet1_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1), Position(4,4));
    // Miss
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Near Collide
    resource->query<Position>().get(bullet1_id) = Position(3.f,0);
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Collide
    resource->query<Position>().get(bullet1_id) = Position(2.99f,0.f);
    task_manager.run_all();
    EXPECT_FALSE(resource->query<Bullet>().has(bullet1_id));
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Reset
    resource->query<BulletHellState>().get(bhs).iframe_time = 0;
    const pid bullet2_id = CreateRectangleBullet(resource, Bullet(5),RectangularCollider(1,2), Position(4,0));

    // Miss
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet2_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Near Collide & Wide pass
    resource->query<Position>().get(bullet2_id) = Position(2.99f,0);
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet2_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Collide
    resource->query<Position>().get(bullet2_id) = Position(0.f,2.99f);
    task_manager.run_all();
    EXPECT_FALSE(resource->query<Bullet>().has(bullet2_id));
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);
}

TEST(Game, bullet_collision2)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, BulletCollision<BulletHell_Syscall>, BulletSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);
    const pid bhs = CreateBulletHellState(resource);

    CreatePlayer(resource, CircularCollider(2));
    pid bullet_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1.5f,1.f), Position(2,-2.3f), {},Rotation(30));
    // Miss
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Collide
    resource->query<Rotation>().get(bullet_id) = Rotation(0);
    task_manager.run_all();
    EXPECT_FALSE(resource->query<Bullet>().has(bullet_id));
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Reset
    resource->query<BulletHellState>().get(bhs).iframe_time = 0;
    bullet_id = CreateRectangleBullet(resource, Bullet(5),RectangularCollider(1,2), Position(-2.5f,0.62f),{},Rotation(0));

    // Miss
    task_manager.run_all();
    EXPECT_TRUE(resource->query<Bullet>().has(bullet_id));
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Collide
    resource->query<Rotation>().get(bullet_id) = Rotation(60);
    task_manager.run_all();
    EXPECT_FALSE(resource->query<Bullet>().has(bullet_id));
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);
}

TEST(Game, bullet_collision3)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, BulletCollision<BulletHell_Syscall>, BulletSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);
    const pid bhs = CreateBulletHellState(resource);

    CreatePlayer(resource, CircularCollider(2));
    // Miss
    const pid bullet1_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1.f,1.f), Position(5,5), Velocity(1),Rotation(-90));
    const pid bullet2_id = CreateRectangleBullet(resource,Bullet(5), RectangularCollider(1.f,1.f), Position(1.5f,-5.2f), Velocity(1.5),Rotation(135));
    //Hit
    const pid bullet3_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1.f,0.5f), Position(3.5f,1.5f), Velocity(0.5),Rotation(210));
    const pid bullet4_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1.f,1.f), Position(0,-5), Velocity(0.75),Rotation(90));
    const pid bullet5_id = CreateRectangleBullet(resource,Bullet(5), RectangularCollider(1.5f,1.f), Position(-4,0), Velocity(0.5),Rotation(-30));

    for (int i=0;i<10;i++)
    {
        task_manager.run_all();
        resource->query<BulletHellState>().get(bhs).iframe_time = 0;
    }

    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));
    EXPECT_TRUE(resource->query<Bullet>().has(bullet2_id));
    EXPECT_FALSE(resource->query<Bullet>().has(bullet3_id));
    EXPECT_FALSE(resource->query<Bullet>().has(bullet4_id));
    EXPECT_FALSE(resource->query<Bullet>().has(bullet5_id));
}

TEST(Game, bullet_collision4)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, DelaySystem<BulletHell_Syscall>, ParticleSystem<BulletHell_Syscall>, ShotDelaySystem<BulletHell_Syscall>, Boomer_System<BulletHell_Syscall>, Laser_System<BulletHell_Syscall>, BulletCollision<BulletHell_Syscall>, BulletSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);
    const pid bhs = CreateBulletHellState(resource);

    CreatePlayer(resource, CircularCollider(2));
    //Missing boom
    CreateBoomerBullet(resource,Bullet(false, 5), Booming(1.5f, 1), Delay(4), Particle(3), Position(4,0), CircularCollider(1.f,1.f));
    //Hit Laser
    CreateLaserBullet(resource,Bullet(false, 5), Laser(4.f, -4.f, 10, 1), Delay(6), Particle(8), Rotation(135), Scale(2),RectangularCollider(1.f,1.f));
    // Hit boom
    CreateBoomerBullet(resource,Bullet(false, 5), Booming(3.f, 1), Delay(11), Particle(11), Position(4,0),CircularCollider(1.f,1.f));
    // Rotate to hit laser
    const pid laser_id = CreateLaserBullet(resource,Bullet(false, 5), Laser(7.f, 0, 10, 1), Delay(20), Particle(20), Rotation(135), Scale(2),RectangularCollider(1.f,1.f));

    task_manager.run_all();
    task_manager.run_all();
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Boomer 1 Fired
    for (int i=0;i<2;i++) task_manager.run_all();
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Laser Fired
    for (int i=0;i<3;i++) task_manager.run_all();
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    // Laser Gone
    for (int i=0;i<2;i++) task_manager.run_all();
    resource->query<BulletHellState>().get(bhs).iframe_time = 0;
    task_manager.run_all();
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);
    task_manager.run_all();

    // Boomer2 Come
    EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);

    for (int i=0;i<3;i++) task_manager.run_all();
    resource->query<BulletHellState>().get(bhs).iframe_time = 0;
    task_manager.run_all();

    // LaserSpin Come
    EXPECT_GT(resource->query<Scale>().get(laser_id).scaleX,0);
    EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);
    for (int i=0;i<5;i++)
    {
        task_manager.run_all();
        resource->query<Rotation>().get(laser_id).angleZ += 5.f;
        EXPECT_EQ(resource->query<Rotation>().get(laser_id).angleZ, 140 + 5*i);
        if (i==4)
            EXPECT_GT(resource->query<BulletHellState>().get(bhs).iframe_time,0);
        else
            EXPECT_LE(resource->query<BulletHellState>().get(bhs).iframe_time,0);
    }

}

TEST(Game, bullet_collision5)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, BulletCollision<BulletHell_Syscall>, BulletSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);
    CreateBulletHellState(resource);

    const pid player_id = CreatePlayer(resource, CircularCollider(2));
    const pid bullet1_id = CreateCircleBullet(resource,Bullet(5), CircularCollider(1), Position(5,5));

    resource->query<Velocity>().get(player_id).vx = 0.75f;
    resource->query<Velocity>().get(player_id).vy = 0.75f;

    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(player_id).x, 0.75f);
    EXPECT_EQ(resource->query<Position>().get(player_id).y, 0.75f);
    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));

    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(player_id).x, 1.5f);
    EXPECT_EQ(resource->query<Position>().get(player_id).y, 1.5f);
    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));

    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(player_id).x, 2.25f);
    EXPECT_EQ(resource->query<Position>().get(player_id).y, 2.25f);
    EXPECT_TRUE(resource->query<Bullet>().has(bullet1_id));

    task_manager.run_all();
    EXPECT_EQ(resource->query<Position>().get(player_id).x, 3.f);
    EXPECT_EQ(resource->query<Position>().get(player_id).y, 3.f);
    EXPECT_FALSE(resource->query<Bullet>().has(bullet1_id));
}

TEST(Game, bullet_pattern1)
{
    TaskManager<BulletHell_Resource, BulletHell_Syscall, PatternSystem<BulletHell_Syscall>, MovementSystem<BulletHell_Syscall>, AccelerationSystem<BulletHell_Syscall>, RotationSystem<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid bullet1_id = CreateBullet1(resource,{}, Position(0,0), Velocity(2.5), Rotation(45));
    const pid bullet2_id = CreateBullet1(resource,{}, Position(2,3));
    AddPattern(resource, bullet1_id, 3, 180, 1, 0.25,0, 2);
    AddPattern(resource, bullet2_id, 3, -90, 0.5, 0,-15);
    AddPattern(resource, bullet1_id, 5, 225, 2, -0.25,15, 0.5, 5);
    AddPattern(resource, bullet2_id, 8, 30, 3);

    task_manager.run_all();
    EXPECT_EQ(resource->query<Patterns>().get(bullet1_id).patterns.front().first, 2);
    task_manager.run_all();
    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).x), 5);
    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).y), 5);
    EXPECT_EQ(resource->query<Velocity>().get(bullet1_id).vx, 2.5);
    EXPECT_EQ(resource->query<Acceleration>().get(bullet1_id).ax, 0);

    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).x * 100)/100.f, 4.3f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).y * 100)/100.f, 5.3f);
    EXPECT_EQ(resource->query<Position>().get(bullet2_id).x, 2);
    EXPECT_EQ(resource->query<Position>().get(bullet2_id).y, 2.5f);
    EXPECT_EQ(resource->query<Velocity>().get(bullet1_id).vx, 1.25);
    EXPECT_EQ(resource->query<Acceleration>().get(bullet1_id).ax, 0.25);
    EXPECT_EQ(resource->query<Rotation>().get(bullet2_id).angleZ, -105);
    EXPECT_EQ(resource->query<AngularVelocity>().get(bullet2_id).v, -15);

    task_manager.run_all();
    task_manager.run_all();

    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).x * 100)/100.f, 1.64f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).y * 100)/100.f, 3.89f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet2_id).x * 100)/100.f, 1.62f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet2_id).y * 100)/100.f, 1.58f);
    EXPECT_EQ(resource->query<Velocity>().get(bullet1_id).vx, 1.75);
    EXPECT_EQ(resource->query<Acceleration>().get(bullet1_id).ax, -0.25);
    EXPECT_EQ(resource->query<Rotation>().get(bullet2_id).angleZ, -135);
    EXPECT_EQ(resource->query<AngularVelocity>().get(bullet2_id).v, -15);

    task_manager.run_all();
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).x * 100)/100.f, 0.38f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet1_id).y * 100)/100.f, -0.33f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet2_id).x * 100)/100.f, 3.43f);
    EXPECT_EQ(round(resource->query<Position>().get(bullet2_id).y * 100)/100.f, 2.48f);
    EXPECT_EQ(resource->query<Velocity>().get(bullet1_id).vx, 1);
    EXPECT_EQ(resource->query<Acceleration>().get(bullet1_id).ax, -0.25);
    EXPECT_EQ(resource->query<Rotation>().get(bullet2_id).angleZ, 15);
    EXPECT_EQ(resource->query<AngularVelocity>().get(bullet2_id).v, -15);

    task_manager.run_all();
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Velocity>().get(bullet1_id).vx, 1.75);
    EXPECT_EQ(resource->query<Acceleration>().get(bullet1_id).ax, -0.25);
}

// -- RHYTHM GAME TESTS --

