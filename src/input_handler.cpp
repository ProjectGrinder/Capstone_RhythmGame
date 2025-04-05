#include "input_handler.h"

using System::InputHandler;

InputHandler InputHandler::_instance;

InputHandler& InputHandler::instance()
{
    return (_instance);
}

void System::InputHandler::poll()
{
    //TODO: event polling logic
}
