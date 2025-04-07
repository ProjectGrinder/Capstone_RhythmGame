#include <Windows.h>

#include "pch.h"
#include "system.h"

TEST(WINDOW, window_creation_test)
{
    System::Window window(GetModuleHandleA(NULL));
    
    uint32_t result = window.create_window();

    EXPECT_EQ(result, ERROR_SUCCESS);
}
