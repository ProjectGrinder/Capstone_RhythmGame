#include "pch.h"
#include "ecs.h"

class ECSTest : public ::testing::Test
{
protected:
    ECS::World world;
    struct TestComponent : ECS::ComponentBase 
    {
        int value;
        TestComponent(int v) : value(v) {}
    };
    ECS::system_function<TestComponent> test_system =
        [](ECS::Entity, TestComponent& c)
        {
            c.value++;
        };
    void SetUp() override
    {
        // To see if needed: ECS::initialize();
    }

    void TearDown() override
    {
        world.reset();
    }
};

TEST_F(ECSTest, create_entity_without_component_test)
{
    ECS::Entity test_entity1 = world.create_entity();
    EXPECT_EQ(test_entity1, 0) << "Entity should be 0.";
    ECS::Entity test_entity2 = world.create_entity();
    EXPECT_EQ(test_entity2, 1) << "Entity should be 1.";
}

TEST_F(ECSTest, create_entity_with_component_test)
{
    ECS::Entity test_entity1 = world.create_entity(TestComponent{0});
    EXPECT_EQ(test_entity1, 0) << "Entity should be 0.";
    ECS::Entity test_entity2 = world.create_entity(TestComponent{1});
    EXPECT_EQ(test_entity2, 1) << "Entity should be 1.";
}

TEST_F(ECSTest, remove_entity_test)
{
    world.create_entity();
    ECS::Entity test_entity2 = world.create_entity();
    world.create_entity();
    ECS::Entity removed_entity = world.remove_entity(test_entity2);
    EXPECT_EQ(removed_entity, test_entity2) << "Input entity and output entity should be the same.";
}

TEST_F(ECSTest, add_system_test)
{
    world.add_system(test_system);
}

TEST_F(ECSTest, add_component_test)
{
    ECS::Entity test_entity = world.create_entity();
    world.add_component<TestComponent>(test_entity, TestComponent{42});
    EXPECT_TRUE(world.has_component<TestComponent>(test_entity)) << "Entity should have component after addition.";
}

TEST_F(ECSTest, remove_component_test)
{
    ECS::Entity test_entity = world.create_entity(TestComponent{ 0 });
    world.remove_component<TestComponent>(test_entity);
    EXPECT_FALSE(world.has_component<TestComponent>(test_entity)) << "Entity should not have component after removal.";
}