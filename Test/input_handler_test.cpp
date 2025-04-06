#include "pch.h"
#include "input_handler.h"

TEST(InputHandler, instance_function_test)
{
	System::InputHandler& inputHandler1 = System::InputHandler::instance();
	EXPECT_NE(&inputHandler1, nullptr) << "InputHandler::instance() returned a reference to nullptr!";
}

TEST(InputHandler, singleton_test)
{
	System::InputHandler& inputHandler1 = System::InputHandler::instance();
	System::InputHandler& inputHandler2 = System::InputHandler::instance();
	EXPECT_EQ(&inputHandler1, &inputHandler2) << "InputHandler::instance() returned a different reference!";
}

class InputHandlerTest : public ::testing::Test {
protected:
    System::InputHandler& handler = System::InputHandler::instance();

    void SetUp() override {
        // before each test, every key is reset to up state
        for (int i = 0; i < 256; ++i) {
            handler.handle(WM_KEYUP, static_cast<BYTE>(i));
        }
    }
};

TEST_F(InputHandlerTest, keydown_keyup_test) {
    BYTE testKey = VK_RETURN;

    handler.handle(WM_KEYDOWN, testKey);
    EXPECT_TRUE(handler.isKeyDown(testKey));

    handler.handle(WM_KEYUP, testKey);
    EXPECT_FALSE(handler.isKeyDown(testKey));
}

TEST_F(InputHandlerTest, keydown_autorepeat_test) {
	BYTE testKey = VK_SPACE;
	handler.handle(WM_KEYDOWN, testKey);
	EXPECT_TRUE(handler.isKeyDown(testKey));
	handler.handle(WM_KEYDOWN, testKey);
	EXPECT_TRUE(handler.isKeyDown(testKey));
	handler.handle(WM_KEYUP, testKey);
	EXPECT_FALSE(handler.isKeyDown(testKey));
}

TEST_F(InputHandlerTest, syskeydown_syskeyup_test) {
    BYTE testKey = VK_MENU;

    handler.handle(WM_SYSKEYDOWN, testKey);
    EXPECT_TRUE(handler.isKeyDown(testKey));

    handler.handle(WM_SYSKEYUP, testKey);
    EXPECT_FALSE(handler.isKeyDown(testKey));
}

TEST_F(InputHandlerTest, irrelevant_message_test) {
    BYTE testKey = 'A';
    handler.handle(WM_MOUSEMOVE, testKey);
    EXPECT_FALSE(handler.isKeyDown(testKey));
}