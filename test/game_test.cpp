#include "pch.h"
#include "system.h"

TEST(Game, change_scene_test)
{
    int value = 0;

    auto test_func1 =
        [&]()
        {
            value = 1;
        };

    auto test_func2 =
        [&]()
        {
            value = 2;
        };

    auto test_func3 =
        [&]()
        {
            value = 3;
        };

    System::Game::change_scene(test_func1);
    EXPECT_EQ(value, 1) << "Game::change_scene() returned incorrect value(it should be able to call and set outside value ie. impure function)[Expect: 1, Actual: " << value << "]";

    System::Game::change_scene(test_func2);
    EXPECT_EQ(value, 2) << "Game::change_scene() returned incorrect value(it should be able to call and set outside value ie. impure function)[Expect: 1, Actual: " << value << "]";

    System::Game::change_scene(test_func3);
    EXPECT_EQ(value, 3) << "Game::change_scene() returned incorrect value(it should be able to call and set outside value ie. impure function)[Expect: 1, Actual: " << value << "]";
}
