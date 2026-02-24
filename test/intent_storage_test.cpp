#include "game.h"
#include "pch.h"
#include "system.h"

using System::ECS::pid;
using System::ECS::ResourceManager;
using System::ECS::Syscall;
using System::ECS::TaskManager;
using System::Render::IntentStorage;

using Game::Render::Camera2D;
using Game::Render::IntentHandle;
using Game::Render::Material;
using Game::Render::Rotation;
using Game::Render::Sprite;
using Game::Render::Text;

using Game::Render::material_intent;
using Game::Render::rotation_intent;
using Game::Render::set_camera;
using Game::Render::sprite_intent;
using Game::Render::text_intent;

using IntentStorageResourceManager = ResourceManager<1000, IntentHandle, Camera2D, Material, Sprite, Rotation, Text>;
using IntentStorageSyscall = Syscall<1000, IntentHandle, Camera2D, Material, Sprite, Rotation, Text>;
using IntentStorageTaskManager = TaskManager<
        IntentStorageResourceManager,
        IntentStorageSyscall,
        set_camera<IntentStorageSyscall>,
        material_intent<IntentStorageSyscall>,
        sprite_intent<IntentStorageSyscall>,
        rotation_intent<IntentStorageSyscall>,
        text_intent<IntentStorageSyscall>>;

// override the singleton

std::unique_ptr<IntentStorage> intent_storage_ptr{};

IntentStorage &IntentStorage::instance()
{
    return *intent_storage_ptr;
}

std::optional<System::Render::DrawIntent> &IntentStorage::get_intent(const size_t slot)
{
    return (instance()._intent_storage[slot]);
}

System::Render::Camera &IntentStorage::get_camera()
{
    return (instance()._camera);
}

class IntentStorageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        intent_storage_ptr = std::make_unique<IntentStorage>();
    }
    void TearDown() override
    {
        intent_storage_ptr.reset();
    }
};

TEST(IntentStorage, test_can_run)
{
    IntentStorageTaskManager task_manager{};
    task_manager.run_all();
    EXPECT_TRUE(true);
}