#include "system/ecs.h"
#include "game.h"
#include "pch.h"

using System::ECS::pid;
using System::ECS::ResourcePool;
using System::ECS::SyscallType;
using System::ECS::TaskManager;
using Game::Battle::BattleState;
using namespace Game::BulletHell;
using namespace Game::Physics;

// -- BULLET HELL TESTS --

using BulletHell_Resource = System::ECS::ResourceManager<1000,Player, Bullet, Position, Scale, Velocity, Rotation, Acceleration, AngularVelocity, Booming, Delay, Laser, Particle, Patterns, CircularCollider, RectangularCollider, Game::Render::Material, BattleState>;

using BulletHell_Syscall = System::ECS::Syscall<1000,Player, Bullet, Position, Scale, Velocity, Rotation, Acceleration, AngularVelocity, Booming, Delay, Laser, Particle, Patterns, CircularCollider, RectangularCollider, Game::Render::Material, BattleState>;

pid CreatePlayer(BulletHell_Resource *resource, CircularCollider cc = {},
    Position pos = Position(0,0), Velocity vel = Velocity(0,0), Rotation rot = Rotation(0),
    Acceleration acc = Acceleration(0,0), AngularVelocity avel = AngularVelocity(0))
{
    const pid id = resource->reserve_process();
    resource->add_resource<Player>(id, Player());
    resource->add_resource<CircularCollider>(id, CircularCollider(cc));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Velocity>(id, Velocity(vel));
    resource->add_resource<Rotation>(id,Rotation(rot));
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

pid CreateBoomerBullet(BulletHell_Resource *resource, Bullet bullet = {}, Booming booming = {},Delay delay = Delay(0),
    Position pos = {}, RectangularCollider rc = {})
{
    const pid id = resource->reserve_process();
    resource->add_resource<Bullet>(id, Bullet(bullet));
    resource->add_resource<Delay>(id, Delay(delay));
    resource->add_resource<Booming>(id, Booming(booming));
    resource->add_resource<Position>(id, Position(pos));
    resource->add_resource<Scale>(id, Scale());
    resource->add_resource<Game::Render::Material>(id,{});
    resource->add_resource<RectangularCollider>(id, RectangularCollider(rc));
    return id;
}

pid CreateBattleState(BulletHell_Resource *resource)
{
    const pid id = resource->reserve_process();
    resource->add_resource<BattleState>(id, BattleState());
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
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>> task_manager{};
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
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, RotationSystem<BulletHell_Syscall>> task_manager{};
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
    TaskManager<BulletHell_Resource, BulletHell_Syscall, MovementSystem<BulletHell_Syscall>, RotationSystem<BulletHell_Syscall>> task_manager{};
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

    const pid id = CreateBullet2(resource,Bullet(3),Delay(2));
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
    TaskManager<BulletHell_Resource, BulletHell_Syscall, DelaySystem<BulletHell_Syscall>, Boomer_System<BulletHell_Syscall>> task_manager{};
    BulletHell_Resource *resource = task_manager.get_rm();
    CreateBattleState(resource);

    const pid id = CreateBoomerBullet(resource, {}, Booming(3,5, 0,2), Delay(3) );
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
    EXPECT_EQ(resource->query<Booming>().get(id).lifetime, 4);
    task_manager.run_all();
    task_manager.run_all();
    task_manager.run_all();
    EXPECT_EQ(resource->query<Delay>().get(id).delay, 0);
    EXPECT_GE(resource->query<Scale>().get(id).scaleX, 2.75f);
    EXPECT_GE(resource->query<Scale>().get(id).scaleY, 2.75f);
    EXPECT_EQ(resource->query<Game::Render::Material>().get(id).color.a, 1);
    EXPECT_EQ(resource->query<Booming>().get(id).lifetime, 1);
}


// -- RHYTHM GAME TESTS --

