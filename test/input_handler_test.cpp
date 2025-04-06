#include "pch.h"
#include "input.h"
#include <Windows.h>

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

class InputHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // before each test, every key is reset to up state
        for (int i = 0; i < 256; ++i) {
            System::Input::set_key_up(i);
        }
    }
};

TEST_F(InputHandlerTest, setup_test) {
	// Check that input is initialized correctly
	for (int i = 0; i < 256; ++i) {
		EXPECT_FALSE(System::Input::is_key_down(i)) << "Key " << i << " should be up.";
	}
}

TEST_F(InputHandlerTest, keydown_keyup_test) {
    BYTE testKey = VK_RETURN;

    System::Input::set_key_down(testKey);
    EXPECT_TRUE(System::Input::is_key_down(testKey));

    System::Input::set_key_up(testKey);
    EXPECT_FALSE(System::Input::is_key_down(testKey));
}

TEST_F(InputHandlerTest, keydown_autorepeat_test) {
    BYTE testKey = VK_SPACE;
    System::Input::set_key_down(testKey);
    EXPECT_TRUE(System::Input::is_key_down(testKey));
    System::Input::set_key_down(testKey);
    EXPECT_TRUE(System::Input::is_key_down(testKey));
    System::Input::set_key_up(testKey);
    EXPECT_FALSE(System::Input::is_key_down(testKey));
}

TEST_F(InputHandlerTest, multiple_keys_test) {
	BYTE testKey1 = VK_UP;
	BYTE testKey2 = VK_DOWN;
    System::Input::set_key_down(testKey1);
	EXPECT_TRUE(System::Input::is_key_down(testKey1));
	EXPECT_FALSE(System::Input::is_key_down(testKey2));
    System::Input::set_key_down(testKey2);
	EXPECT_TRUE(System::Input::is_key_down(testKey1));
	EXPECT_TRUE(System::Input::is_key_down(testKey2));
    System::Input::set_key_up(testKey1);
	EXPECT_FALSE(System::Input::is_key_down(testKey1));
	EXPECT_TRUE(System::Input::is_key_down(testKey2));
    System::Input::set_key_up(testKey2);
	EXPECT_FALSE(System::Input::is_key_down(testKey1));
	EXPECT_FALSE(System::Input::is_key_down(testKey2));
}