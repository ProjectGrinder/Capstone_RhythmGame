#pragma once

namespace Game::Render
{
    struct Animator
    {
        unsigned int frame;
        Animator() : frame(0)
        {}
        explicit Animator(const unsigned int frame) : frame(frame)
        {}
    };
}