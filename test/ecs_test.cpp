#include "pch.h"
#include "ecs.h"

struct TestComponent : ECS::ComponentBase
{
    int value;
    TestComponent(int v) : value(v) {}
};
void test_system(ECS::entity_id, TestComponent& c)
{
    c.value++;
}

class ECSTest : public ::testing::Test
{
protected:

    ECS::World world;
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
    ECS::entity_id test_entity1 = world.create_entity();
    EXPECT_EQ(test_entity1, 0) << "entity_id should be 0.";
    ECS::entity_id test_entity2 = world.create_entity();
    EXPECT_EQ(test_entity2, 1) << "entity_id should be 1.";
}

TEST_F(ECSTest, create_entity_with_component_test)
{
    ECS::entity_id test_entity1 = world.create_entity(TestComponent{0});
    EXPECT_EQ(test_entity1, 0) << "entity_id should be 0.";
    ECS::entity_id test_entity2 = world.create_entity(TestComponent{1});
    EXPECT_EQ(test_entity2, 1) << "entity_id should be 1.";
}

TEST_F(ECSTest, remove_entity_test)
{
    world.create_entity();
    ECS::entity_id test_entity2 = world.create_entity();
    world.create_entity();
    ECS::entity_id removed_entity = world.remove_entity(test_entity2);
    EXPECT_EQ(removed_entity, test_entity2) << "Input entity and output entity should be the same.";
}

TEST_F(ECSTest, add_system_test)  
{  
   // Fix: Ensure the lambda matches the expected signature of add_system  
    world.add_system<TestComponent>(test_system);
}

TEST_F(ECSTest, add_component_test)
{
    ECS::entity_id test_entity = world.create_entity();
    world.add_component<TestComponent>(test_entity, TestComponent{42});
    EXPECT_TRUE(world.has_component<TestComponent>(test_entity)) << "entity_id should have component after addition.";
}

TEST_F(ECSTest, remove_component_test)
{
    ECS::entity_id test_entity = world.create_entity(TestComponent{ 0 });
    world.remove_component<TestComponent>(test_entity);
    EXPECT_FALSE(world.has_component<TestComponent>(test_entity)) << "entity_id should not have component after removal.";
}