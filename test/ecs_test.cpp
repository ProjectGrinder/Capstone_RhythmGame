#include "system/ecs.h"
#include "pch.h"

using System::ECS::pid;
using System::ECS::ResourceManager;
using System::ECS::ResourcePool;
using System::ECS::Syscall;
using System::ECS::SyscallType;
using System::ECS::TaskManager;

struct test_component
{
    test_component() : value(0)
    {}
    explicit test_component(const int value) : value(value)
    {}
    int value;
};

struct test_component_2
{
    test_component_2() : value(0)
    {}
    explicit test_component_2(const int value) : value(value)
    {}
    int value;
};


using TestResource = ResourceManager<1000, test_component, test_component_2>;
using TestSyscall = Syscall<1000, test_component, test_component_2>;

void test_system([[maybe_unused]] TestSyscall &syscall, System::ECS::Query<test_component> &query)
{
    for (auto &entry: query)
    {
        auto &comp = entry.get<test_component>();
        comp.value++;
    }
}

void test_system_2([[maybe_unused]] TestSyscall &syscall, System::ECS::Query<test_component> &query)
{
    for (auto &[id, comps]: query)
    {
        auto &comp = comps.get<test_component>();
        comp.value *= 2;
    }
}

void test_system_3(TestSyscall &syscall, System::ECS::Query<test_component> &query)
{
    for (auto &entry: query)
    {
        syscall.remove_component<test_component>(entry.id);
    }
}

void test_system_4(
        [[maybe_unused]] TestSyscall &syscall,
        System::ECS::Query<test_component> &query,
        System::ECS::Query<test_component_2> &query_2)
{
    for (auto &entry: query)
    {
        int sum = 0;
        for (auto &entry_2: query_2)
        {
            sum += entry_2.get<test_component_2>().value;
        }
        entry.get<test_component>().value = sum;
    }
}

// Test ResourceManager

TEST(ECS, add_component_test)
{
    TestResource resource;
    const pid id = resource.reserve_process();
    resource.add_resource<test_component>(id, test_component{1});
    EXPECT_EQ(resource.query<test_component>().get(id).value, 1);
    EXPECT_TRUE(resource.query<test_component>().has(id));
}

TEST(ECS, add_multiple_components_test)
{
    TestResource resource;
    const pid id_1 = resource.reserve_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    const pid id_2 = resource.reserve_process();
    resource.add_resource<test_component>(id_2, test_component{2});
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_EQ(resource.query<test_component>().get(id_2).value, 2);
}

TEST(ECS, remove_component_test)
{
    TestResource resource;
    const pid id = resource.reserve_process();
    resource.add_resource<test_component>(id, test_component{1});
    EXPECT_TRUE(resource.query<test_component>().has(id));
    resource.remove_resource<test_component>(id);
    EXPECT_FALSE(resource.query<test_component>().has(id));
}

TEST(ECS, layer_components_test)
{
    TestResource resource;
    const pid id_1 = resource.reserve_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    resource.add_resource<test_component_2>(id_1, test_component_2{2});
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_EQ(resource.query<test_component_2>().get(id_1).value, 2);
}

TEST(ECS, delete_resource_test)
{
    TestResource resource;
    const pid id_1 = resource.reserve_process();
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
    TestResource resource;
    TestSyscall syscall{resource};
    const pid id_1 = resource.reserve_process();
    syscall.add_component(id_1, test_component{1});
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    syscall.exec();
    EXPECT_EQ(resource.query<test_component>().get(id_1).value, 1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_create_entity)
{
    TestResource resource;
    TestSyscall syscall{resource};
    const pid id_1 = syscall.create_entity(test_component{1});
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    syscall.exec();
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_remove_component)
{
    TestResource resource;
    TestSyscall syscall{resource};
    const pid id_1 = resource.reserve_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    syscall.remove_component<test_component>(id_1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
    syscall.exec();
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
}

TEST(ECS, exec_remove_entity)
{
    TestResource resource;
    TestSyscall syscall{resource};
    const pid id_1 = resource.reserve_process();
    resource.add_resource<test_component>(id_1, test_component{1});
    resource.add_resource<test_component_2>(id_1, test_component_2{2});
    syscall.remove_entity(id_1);
    EXPECT_TRUE(resource.query<test_component>().has(id_1));
    EXPECT_TRUE(resource.query<test_component_2>().has(id_1));
    syscall.exec();
    EXPECT_FALSE(resource.query<test_component>().has(id_1));
    EXPECT_FALSE(resource.query<test_component_2>().has(id_1));
}

// Test TaskManager

TEST(ECS, system_run_test)
{

    TaskManager<TestResource, TestSyscall, test_system> task_manager(TestResource{});
    TestResource *resource = task_manager.get_rm();

    const pid id_1 = resource->reserve_process();
    resource->add_resource<test_component>(id_1, test_component{1});

    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 1);
    task_manager.run_all();
    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 2);
}

TEST(ECS, system_sequence_test)
{
    TaskManager<TestResource, TestSyscall, test_system, test_system_2> task_manager(TestResource{});
    TestResource *resource = task_manager.get_rm();

    const pid id_1 = resource->reserve_process();
    resource->add_resource<test_component>(id_1, test_component{1});

    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 1);
    task_manager.run_all();
    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 4); // (1+1)*2
}

TEST(ECS, system_defer_syscall_test)
{
    TaskManager<TestResource, TestSyscall, test_system_3, test_system_2, test_system> task_manager(TestResource{});
    TestResource *resource = task_manager.get_rm();

    const pid id_1 = resource->reserve_process();
    resource->add_resource<test_component>(id_1, test_component{1});
    EXPECT_TRUE(resource->query<test_component>().has(id_1));
    task_manager.run_all();
    EXPECT_FALSE(resource->query<test_component>().has(id_1));
}

TEST(ECS, system_interaction_test)
{
    TaskManager<TestResource, TestSyscall, test_system_4> task_manager(TestResource{});
    TestResource *resource = task_manager.get_rm();

    const pid id_1 = resource->reserve_process();
    resource->add_resource<test_component>(id_1, test_component{999});

    // No test_component_2 in TaskManager: component should yield 0
    task_manager.run_all();
    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 0);

    const pid id_2 = resource->reserve_process();
    resource->add_resource<test_component_2>(id_2, test_component_2{1});

    // component should yield 1
    task_manager.run_all();
    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 1);

    const pid id_3 = resource->reserve_process();
    resource->add_resource<test_component_2>(id_3, test_component_2{2});

    // component should yield 3
    task_manager.run_all();
    EXPECT_EQ(resource->query<test_component>().get(id_1).value, 3);
}
