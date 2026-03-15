#include "system/ecs.h"
#include "game.h"
#include "pch.h"

using System::ECS::pid;
using System::ECS::ResourceManager;
using System::ECS::ResourcePool;
using System::ECS::Syscall;
using System::ECS::SyscallType;
using System::ECS::TaskManager;
using Game::Battle::BattleState;

using namespace Game::Rhythm;

using NoteResource = ResourceManager<1000, Lane, Timing, TimingEnd, NoteSpeed, HoldActive, NoteType, BattleState>;

using NoteSyscall = Syscall<1000, Lane, Timing, TimingEnd, NoteSpeed, HoldActive, NoteType, BattleState>;

pid CreateNote(NoteResource *resource,
    Lane lane = Lane(0), Timing timing = Timing(1000), TimingEnd timing_end = TimingEnd(0),
    NoteSpeed note_speed = NoteSpeed(1.0f), NoteType note_type = NoteType(0))
{
    const pid id = resource->reserve_process();

    resource->add_resource<Lane>(id, Lane(lane));
    resource->add_resource<Timing>(id, Timing(timing));
    resource->add_resource<TimingEnd>(id, TimingEnd(timing_end));
    resource->add_resource<NoteSpeed>(id, NoteSpeed(note_speed));
    resource->add_resource<NoteType>(id, NoteType(note_type));
    return (id);
}

// Test Entity Instantiation

TEST(Game, instantiate_note)
{
    TaskManager<NoteResource, NoteSyscall> task_manager{};
    NoteResource *resource = task_manager.get_rm();

    const pid id = CreateNote(resource);
    EXPECT_TRUE(resource->query<Lane>().has(id));
    EXPECT_TRUE(resource->query<Timing>().has(id));
    EXPECT_TRUE(resource->query<TimingEnd>().has(id));
    EXPECT_TRUE(resource->query<NoteSpeed>().has(id));
    EXPECT_FALSE(resource->query<HoldActive>().get(id).hold_active);
    EXPECT_TRUE(resource->query<NoteType>().has(id));
}