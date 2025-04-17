#pragma once

#include "input.h"

namespace System
{
    class Context;

    class State
    {
    private:
        Context* context;

    public:
		virtual ~State() = default;

        virtual void handle_input(Input* input) = 0;
		virtual void update() = 0;
		virtual void render() = 0;

        void set_context(Context* context)
        {
            this->context = context;
        }

		std::string get_name()
		{
            return (typeid(*this).name());
		}
    };

    class Context
    {
    private:
        State* state;

    public:
        Context(State* state) : state(nullptr)
        {
            this->set_state(state);
        }

        ~Context()
        {
            delete (state);
        }

        void set_state(State* state)
        {
            if (this->state != nullptr)
            {
                delete (this->state);
            }
            this->state = state;
            this->state->set_context(this);
        }

        State* get_state() const
        {
            return (state);
        }
    };

    class MenuState : public State
    {
	public:
        void handle_input(Input* input) override {}
		void update() override {}
		void render() override {}
    };
}