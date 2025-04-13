#include "pch.h"
#include "ecs.h"

class ECSTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // To see if needed: ECS::initialize();
    }

    void TearDown() override
    {
        ECS::clear_entities();
        ECS::clear_components();
        ECS::clear_systems();
    }
};

TEST_F(ECSTest, create_entity_with_component_test)
{
    struct TestComponent {};
    ECS::Entity test_entity = ECS::create_entity<TestComponent>();
    EXPECT_TRUE(ECS::has_entity(test_entity)) << "Entity should exist in world after creation.";
}

TEST_F(ECSTest, create_entity_without_component_test)
{
    ECS::Entity test_entity = ECS::create_entity<>();
    EXPECT_TRUE(ECS::has_entity(test_entity)) << "Entity should exist in world after creation.";
}

TEST_F(ECSTest, remove_entity_test)
{
    struct TestComponent {};
    ECS::Entity test_entity = ECS::create_entity<TestComponent>();
    ECS::Entity removed_entity = ECS::remove_entity(test_entity);
    EXPECT_EQ(test_entity, removed_entity) << "Input entity and output entity should be the same.";
    EXPECT_FALSE(ECS::has_entity(test_entity)) << "Entity should not exist in world after destruction.";
}

TEST_F(ECSTest, add_system_test)
{
    struct TestComponent {};

    std::function<void(ECS::EntityView<TestComponent>&)> test_system =
        [](ECS::EntityView<TestComponent>&)
        {
            /* no-op */
        };

    ECS::add_system(test_system);
    EXPECT_TRUE(ECS::has_system(test_system)) << "System should exist in the world.";
}

TEST_F(ECSTest, remove_system_test)
{
    struct TestComponent {};

    std::function<void(ECS::EntityView<TestComponent>&)> test_system =
        [](ECS::EntityView<TestComponent>&)
        {
            /* no-op */
        };

    ECS::add_system(test_system);
    ECS::remove_system(test_system);
    EXPECT_FALSE(ECS::has_system(test_system)) << "System should not exist in the world after removal.";
}

TEST_F(ECSTest, add_component_test)
{
    struct TestComponent {};
    ECS::Entity test_entity = ECS::create_entity<>();
    ECS::add_component<TestComponent>(test_entity);
    EXPECT_TRUE(ECS::has_component<TestComponent>(test_entity)) << "Entity should have component after addition.";
}

TEST_F(ECSTest, remove_component_test)
{
    struct TestComponent {};
    ECS::Entity test_entity = ECS::create_entity<TestComponent>();
    ECS::remove_component<TestComponent>(test_entity);
    EXPECT_FALSE(ECS::has_component<TestComponent>(test_entity)) << "Entity should not have component after removal.";
}

TEST_F(ECSTest, clear_entities_test)
{
    ECS::Entity test_entity1 = ECS::create_entity<>();
    ECS::Entity test_entity2 = ECS::create_entity<>();
    ECS::clear_entities();
    EXPECT_FALSE(ECS::has_entity(test_entity1)) << "Entity should not exist in world after clearing.";
    EXPECT_FALSE(ECS::has_entity(test_entity2)) << "Entity should not exist in world after clearing.";
}
