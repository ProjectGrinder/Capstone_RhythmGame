#pragma once

namespace Game::Rhythm
{
    struct KeyInput
    {
        bool input1;
        bool input2;
        bool input3;
        bool input4;
        KeyInput() : input1(false), input2(false), input3(false), input4(false)
        {}
    };
}
