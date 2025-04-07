#include "pch.h"
#include "system.h"

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

TEST(OS, stop_test)
{
    System::OS::stop();
    EXPECT_FALSE(System::OS::is_running());
}

TEST(OS, set_system_precision_test)
{
    uint32_t error;

    error = System::OS::set_system_precision(5);
    EXPECT_EQ(error, ERROR_SUCCESS);
    EXPECT_EQ(System::OS::get_system_precision(), 5);

    error = System::OS::set_system_precision(-1);
    EXPECT_EQ(error, ERROR_INVALID_PARAMETER);

    error = System::OS::set_system_precision(0);
    EXPECT_EQ(error, ERROR_INVALID_PARAMETER);

    EXPECT_EQ(System::OS::get_system_precision(), 5);
}
