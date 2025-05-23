#include <Windows.h>
#include <ranges>
#include <algorithm>

#include "pch.h"
#include "system.h"

TEST(Input, instance_function_test)
{
    System::Input& inputHandler1 = System::Input::instance();
    EXPECT_NE(&inputHandler1, nullptr) << "Input::instance() returned a reference to nullptr!";
}

TEST(Input, singleton_test)
{
    System::Input& inputHandler1 = System::Input::instance();
    System::Input& inputHandler2 = System::Input::instance();
    EXPECT_EQ(&inputHandler1, &inputHandler2) << "Input::instance() returned a different reference!";
}

class InputHandlerTest : public ::testing::Test 
{
    protected:
        void SetUp() override 
        {
            std::ranges::for_each
            (
                std::views::iota(0, 256) |
                std::views::transform([](int key) {return (uint8_t)key;}),
                System::Input::set_key_up
            );
        }
};

TEST_F(InputHandlerTest, setup_test) 
{
    std::ranges::for_each
    (
        std::views::iota(0, 256), [](int test_key) 
        { 
            EXPECT_FALSE(System::Input::is_key_down((uint8_t)test_key));
        }
    );
}

TEST_F(InputHandlerTest, keydown_keyup_test) 
{
    BYTE test_key = VK_RETURN;

    System::Input::set_key_down(test_key);
    EXPECT_TRUE(System::Input::is_key_down(test_key));

    System::Input::set_key_up(test_key);
    EXPECT_FALSE(System::Input::is_key_down(test_key));
}

TEST_F(InputHandlerTest, keydown_autorepeat_test) 
{
    BYTE test_key = VK_SPACE;
    System::Input::set_key_down(test_key);
    EXPECT_TRUE(System::Input::is_key_down(test_key));
    System::Input::set_key_down(test_key);
    EXPECT_TRUE(System::Input::is_key_down(test_key));
    System::Input::set_key_up(test_key);
    EXPECT_FALSE(System::Input::is_key_down(test_key));
}

TEST_F(InputHandlerTest, multiple_keys_test) 
{
    BYTE test_key1 = VK_UP;
    BYTE test_key2 = VK_DOWN;
    System::Input::set_key_down(test_key1);
    EXPECT_TRUE(System::Input::is_key_down(test_key1));
    EXPECT_FALSE(System::Input::is_key_down(test_key2));
    System::Input::set_key_down(test_key2);
    EXPECT_TRUE(System::Input::is_key_down(test_key1));
    EXPECT_TRUE(System::Input::is_key_down(test_key2));
    System::Input::set_key_up(test_key1);
    EXPECT_FALSE(System::Input::is_key_down(test_key1));
    EXPECT_TRUE(System::Input::is_key_down(test_key2));
    System::Input::set_key_up(test_key2);
    EXPECT_FALSE(System::Input::is_key_down(test_key1));
    EXPECT_FALSE(System::Input::is_key_down(test_key2));
}

TEST(Input, mouse_position_test)
{
    uint16_t x = 100;
    uint16_t y = 200;
    System::Input::set_mouse_position(x, y);
    Math::Vector2<uint16_t> mouse_position = System::Input::get_mouse_position();
    EXPECT_EQ(mouse_position.x, x);
    EXPECT_EQ(mouse_position.y, y);
}
