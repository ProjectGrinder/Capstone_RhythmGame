#include "pch.h"
#include "../src/os.cpp"

TEST(OS, instance_function_test)
{
    System::OS& os1 = System::OS::instance();
    EXPECT_NE(&os1, nullptr) << "OS::instance() returned a reference to nullptr!";
}

TEST(OS, singleton_test)
{
    System::OS& os1 = System::OS::instance();
    System::OS& os2 = System::OS::instance();
    EXPECT_EQ(&os1, &os2) << "OS::instance() returned a different reference!";
}

TEST(OS, not_running_by_default_test)
{
    EXPECT_FALSE(OS::is_running());
}

TEST(OS, stop_test)
{
    OS::stop();
    EXPECT_FALSE(OS::is_running());
}