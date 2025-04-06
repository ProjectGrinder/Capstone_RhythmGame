#include "pch.h"
#include "os.h"

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
    EXPECT_FALSE(System::OS::is_running());
}

TEST(OS, run)
{
    int result = System::OS::run();
    EXPECT_EQ(result, 0);
}

TEST(OS, stop_test)
{
    System::OS::stop();
    EXPECT_FALSE(System::OS::is_running());
}
