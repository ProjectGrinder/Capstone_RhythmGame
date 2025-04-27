#include "pch.h"
#include "ecs.h"

struct TestComponent : ECS::ComponentBase
{
    int value;
    TestComponent(int v) : value(v) {}
};
struct TestComponent2 : ECS::ComponentBase
{
    int value;
    TestComponent2(int v) : value(v) {}
};

void test_system(ECS::EntityMap<TestComponent> matched_entities)
{
    for (auto& [_, components] : matched_entities)
    {
        auto& [test_component] = components;
        test_component.value++;
    }
}

void duplicate_test_system(ECS::EntityMap<TestComponent> matched_entities)
{
    for (auto& [_, components] : matched_entities)
    {
        auto& [test_component] = components;
        test_component.value *= 2;
    }
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

// Entity Management

TEST_F(ECSTest, create_entity_test)
{
    ECS::entity_id entity = world.create_entity<>();
    EXPECT_EQ(entity, 0);
    EXPECT_TRUE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 1);
}

TEST_F(ECSTest, create_multiple_entities_test)
{
    ECS::entity_id entity1 = world.create_entity<>();
    ECS::entity_id entity2 = world.create_entity<>();
    ECS::entity_id entity3 = world.create_entity<>();
    EXPECT_EQ(entity1, 0);
    EXPECT_EQ(entity2, 1);
    EXPECT_EQ(entity3, 2);
    EXPECT_TRUE(world.has_entity(entity1));
    EXPECT_TRUE(world.has_entity(entity2));
    EXPECT_TRUE(world.has_entity(entity3));
    EXPECT_EQ(world.entity_count(), 3);
}

TEST_F(ECSTest, create_entity_with_component_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_EQ(entity, 0);
    EXPECT_TRUE(world.has_entity(entity));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    EXPECT_EQ(world.entity_count(), 1);
}

TEST_F(ECSTest, create_entity_mixed_component_test)
{
    ECS::entity_id entity1 = world.create_entity<TestComponent>(TestComponent(5));
    ECS::entity_id entity2 = world.create_entity<TestComponent>(TestComponent(10));
    ECS::entity_id entity3 = world.create_entity<>();
    EXPECT_EQ(entity1, 0);
    EXPECT_EQ(entity2, 1);
    EXPECT_EQ(entity3, 2);
    EXPECT_TRUE(world.has_entity(entity1));
    EXPECT_TRUE(world.has_entity(entity2));
    EXPECT_TRUE(world.has_entity(entity3));
    EXPECT_TRUE(world.has_component<TestComponent>(entity1));
    EXPECT_TRUE(world.has_component<TestComponent>(entity2));
    EXPECT_FALSE(world.has_component<TestComponent>(entity3));
    EXPECT_EQ(world.get_component<TestComponent>(entity1).value, 5);
    EXPECT_EQ(world.get_component<TestComponent>(entity2).value, 10);
    EXPECT_THROW(world.get_component<TestComponent>(entity3), std::out_of_range);
    EXPECT_EQ(world.entity_count(), 3);
}

TEST_F(ECSTest, remove_entity_test)
{
    ECS::entity_id entity = world.create_entity<>();
    EXPECT_TRUE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 1);
    world.remove_entity(entity);
    EXPECT_FALSE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 0);
}

TEST_F(ECSTest, duplicate_remove_entity_test)
{
    ECS::entity_id entity = world.create_entity<>();
    EXPECT_TRUE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 1);
    world.remove_entity(entity);
    EXPECT_FALSE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 0);
    world.remove_entity(entity);
    EXPECT_FALSE(world.has_entity(entity));
    EXPECT_EQ(world.entity_count(), 0);
}

TEST_F(ECSTest, remove_nonexistent_entity_test)
{
    world.create_entity<>();
    world.create_entity<>();
    world.create_entity<>();
    world.create_entity<>();
    world.create_entity<>();
    
    ECS::entity_id nonexistent_entity = 100;
    world.remove_entity(nonexistent_entity);
    EXPECT_EQ(world.entity_count(), 5);
    EXPECT_FALSE(world.has_entity(nonexistent_entity));
}

TEST_F(ECSTest, remove_entity_with_component_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    world.remove_entity(entity);
    EXPECT_FALSE(world.has_entity(entity));
    EXPECT_THROW(world.has_component<TestComponent>(entity), std::out_of_range);
}

TEST_F(ECSTest, clear_entity_test)
{
    ECS::entity_id entity1 = world.create_entity<TestComponent>(TestComponent(5));
    ECS::entity_id entity2 = world.create_entity<TestComponent>(TestComponent(10));
    EXPECT_EQ(world.entity_count(), 2);
    EXPECT_TRUE(world.has_entity(entity1));
    EXPECT_TRUE(world.has_entity(entity2));
    world.clear_entities();
    EXPECT_EQ(world.entity_count(), 0);
    EXPECT_FALSE(world.has_entity(entity1));
    EXPECT_FALSE(world.has_entity(entity2));
}

// Component Management

TEST_F(ECSTest, add_component_test)
{
    ECS::entity_id entity = world.create_entity<>();
    world.add_component<TestComponent>(entity, TestComponent(5));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
}

TEST_F(ECSTest, overwrite_component_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.add_component<TestComponent>(entity, TestComponent(10));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 10);
}

TEST_F(ECSTest, remove_component_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.remove_component<TestComponent>(entity);
    EXPECT_FALSE(world.has_component<TestComponent>(entity));
    EXPECT_THROW(world.get_component<TestComponent>(entity), std::out_of_range);
}

TEST_F(ECSTest, duplicate_remove_component_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_TRUE(world.has_component<TestComponent>(entity));
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.remove_component<TestComponent>(entity);
    EXPECT_FALSE(world.has_component<TestComponent>(entity));
    EXPECT_THROW(world.get_component<TestComponent>(entity), std::out_of_range);
    world.remove_component<TestComponent>(entity);
    EXPECT_FALSE(world.has_component<TestComponent>(entity));
    EXPECT_THROW(world.get_component<TestComponent>(entity), std::out_of_range);
}

TEST_F(ECSTest, add_component_to_nonexistent_entity_test)
{
    EXPECT_THROW(world.add_component<TestComponent>(100, TestComponent(5)), std::out_of_range);
    EXPECT_THROW(world.has_component<TestComponent>(100), std::out_of_range);

}

TEST_F(ECSTest, remove_component_from_nonexistent_entity_test)
{
    EXPECT_THROW(world.remove_component<TestComponent>(100), std::out_of_range);
    EXPECT_THROW(world.has_component<TestComponent>(100), std::out_of_range);
}

TEST_F(ECSTest, remove_component_from_entity_with_no_component_test)
{
    ECS::entity_id entity = world.create_entity<>();
    EXPECT_FALSE(world.has_component<TestComponent>(entity));
    EXPECT_THROW(world.get_component<TestComponent>(entity), std::out_of_range);
    world.remove_component<TestComponent>(entity);
    EXPECT_FALSE(world.has_component<TestComponent>(entity));
    EXPECT_THROW(world.get_component<TestComponent>(entity), std::out_of_range);
}

// System Management

TEST_F(ECSTest, add_system_test)
{
    EXPECT_EQ(world.system_count(), 0);
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
}

TEST_F(ECSTest, remove_system_test)
{
    EXPECT_EQ(world.system_count(), 0);
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
    world.remove_system(test_system);
    EXPECT_EQ(world.system_count(), 0);
}

TEST_F(ECSTest, remove_nonexistent_system_test)
{
    EXPECT_EQ(world.system_count(), 0);
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
    world.remove_system(test_system);
    EXPECT_EQ(world.system_count(), 0);
    world.remove_system(test_system);
    EXPECT_EQ(world.system_count(), 0);
}

TEST_F(ECSTest, run_system_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 6);
}

TEST_F(ECSTest, run_multiple_systems_in_order_test)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    world.add_system(test_system);
    world.add_system(duplicate_test_system);
    EXPECT_EQ(world.system_count(), 2);
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 12); // (5 + 1) * 2

    world.remove_system(test_system);
    world.remove_system(duplicate_test_system);
    EXPECT_EQ(world.system_count(), 0);

    world.add_component<TestComponent>(entity, TestComponent(5));
    world.add_system(duplicate_test_system);
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 2);
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 5);
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 11); // (5 * 2) + 1
}

TEST_F(ECSTest, run_system_with_multiple_entities_test)
{
    ECS::entity_id entity1 = world.create_entity<TestComponent>(TestComponent(5));
    ECS::entity_id entity2 = world.create_entity<TestComponent>(TestComponent(10));
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
    EXPECT_EQ(world.get_component<TestComponent>(entity1).value, 5);
    EXPECT_EQ(world.get_component<TestComponent>(entity2).value, 10);
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity1).value, 6);
    EXPECT_EQ(world.get_component<TestComponent>(entity2).value, 11);
}

TEST_F(ECSTest, run_system_with_component_added_later_test)
{
    world.add_system(test_system);
    ECS::entity_id entity1 = world.create_entity<TestComponent>(TestComponent(5));
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity1).value, 6);

    ECS::entity_id entity2 = world.create_entity<>();
    world.add_component<TestComponent>(entity2, TestComponent(10));
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity1).value, 7);
    EXPECT_EQ(world.get_component<TestComponent>(entity2).value, 11);
}

TEST_F(ECSTest, add_duplicate_systems)
{
    ECS::entity_id entity = world.create_entity<TestComponent>(TestComponent(5));
    EXPECT_EQ(world.system_count(), 0);
    world.add_system(test_system);
    world.add_system(test_system);
    EXPECT_EQ(world.system_count(), 1);
    world.run_systems();
    EXPECT_EQ(world.get_component<TestComponent>(entity).value, 6);
}

