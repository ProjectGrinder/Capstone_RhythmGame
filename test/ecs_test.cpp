#include "pch.h"
#include "system/ecs_types.h"

struct test_component
{
    int value;
};

struct test_component_2
{
    int value;
};

void test_system([[maybe_unused]] System::pid id, test_component& comp)
{
    comp.value = 1;
}

void test_system_2([[maybe_unused]] System::pid id, test_component& comp)
{
    comp.value = 2;
}

TEST(ECS, add_component_test)
{
    using TestResource = System::ResourceManager<1000, test_component>;
    TestResource resource;
    test_component comp;
    const System::pid id = resource.add_process();
    comp.value = 1;
    auto pool = resource.query<test_component>();
    pool.add(id, comp);
    EXPECT_EQ(pool.get(id).value, 1);
}