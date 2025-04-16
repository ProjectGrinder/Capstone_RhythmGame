#include "pch.h"
#include "state.h"
#include "system.h"

TEST(State, context_test)
{
	System::Context context(nullptr);
	EXPECT_EQ(context.get_state(), nullptr) << "Context::get_state() returned a non-null pointer!";
}

class MockState : public System::State
{
protected:
	System::Context* context;
	void handle_input(System::Input* input) override {}
	void update() override
	{
		this->context->set_state(new System::MenuState());
	}
	void render() override {}
};

TEST(State, set_state_test)
{
   System::Context context(nullptr);
   context.set_state(new MockState());
   EXPECT_NE(context.get_state(), nullptr) << "Context::get_state() returned a null pointer!";
   System::State* state = context.get_state();
   EXPECT_EQ(state->get_name(), "MockState") << "Context::get_name() returned an unexpected name!";
}

TEST(State, update_function_test)
{
	System::Context context(new MockState());
	System::State* state = context.get_state();
	state->update();
	EXPECT_EQ(state->get_name(), "MenuState") << "State::update() did not change the state correctly!";
}

TEST(State, delete_test)
{
	System::Context context(new MockState());
	System::State* state = context.get_state();
	delete state;
	EXPECT_EQ(context.get_state(), nullptr) << "Context::get_state() did not return a null pointer after deletion!";
}