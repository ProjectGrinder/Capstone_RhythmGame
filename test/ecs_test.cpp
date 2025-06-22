#include "pch.h"
#include "system/ecs_types.h"

struct test_component
{
    test_component(): value(0){}
    explicit test_component(const int value) : value(value)
    {}
    int value;
};

struct test_component_2
{
    test_component_2(): value(0){}
    explicit test_component_2(const int value) : value(value)
    {}
    int value;
};

void test_system([[maybe_unused]] System::pid id, test_component& comp)
{
    comp.value += 1;
}

void test_system_2([[maybe_unused]] System::pid id, test_component& comp)
{
    comp.value *= 2;
}

// Test ResourceManager

TEST(ECS, add_component_test)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    const System::pid id = resource.add_process();
    resource.add_resource<test_component>(id, test_component{1});
    EXPECT_EQ(resource.query<test_component>().get(id).value, 1);
    EXPECT_TRUE(resource.query<test_component>().has(id));
}

TEST(ECS, add_multiple_components_test)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    const System::pid id_2 = resource.add_process();
    resource.add_resource<test_component>(id_2, test_component{2});
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_EQ(resource.query<test_component>().get(id_2).value, 2);
}

TEST(ECS, layer_components_test)
{
    using TestResource = System::ResourceManager<1000, test_component, test_component_2>;
    TestResource resource;
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    resource.add_resource<test_component_2>(id_1, test_component_2{2});
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_EQ(resource.query<test_component_2>().get(id_1).value, 2);
}

TEST(ECS, delete_resource_test)
{
    using TestResource = System::ResourceManager<1000, test_component, test_component_2>;
    TestResource resource;
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    resource.add_resource<test_component_2>(id_1, test_component_2{2});
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_EQ(resource.query<test_component_2>().get(id_1).value, 2);
    resource.delete_entity(id_1);
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    EXPECT_FALSE(resource.query<test_component_2>().has(id_1));
}

// Test Syscall

TEST(ECS, exec_add_component)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    System::Syscall<1000, test_component> syscall{};
    const System::pid id_1 = resource.add_process();
    syscall.add_component(id_1, test_component{1});
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    syscall.exec(resource);
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_create_entity)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    System::Syscall<1000, test_component> syscall{};
    const System::pid id_1 = syscall.create_entity(resource, test_component{1});
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    syscall.exec(resource);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_remove_component)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    System::Syscall<1000, test_component> syscall{};
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    syscall.remove_component<test_component>(id_1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
    syscall.exec(resource);
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_remove_entity)
{
    using TestResource = System::ResourceManager<1000, test_component, test_component_2>;
    TestResource resource;
    System::Syscall<1000, test_component,test_component_2> syscall{};
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    resource.add_resource<test_component_2>(id_1, test_component_2{2});
    syscall.remove_entity(id_1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
    EXPECT_TRUE(resource.query<test_component_2>().has(id_1));
    syscall.exec(resource);
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    EXPECT_FALSE(resource.query<test_component_2>().has(id_1));
}

// Test TaskManager

TEST(ECS, system_run_test)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    System::TaskManager<TestResource, test_system> task_manager(resource);
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    task_manager.run_all();
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 2);
}

TEST(ECS, system_sequence_test)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    const System::pid id_1 = resource.add_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    System::TaskManager<TestResource, test_system, test_system_2> task_manager(resource);
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    task_manager.run_all();
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 4); // (1+1)*2
}