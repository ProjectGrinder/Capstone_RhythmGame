#pragma once

namespace Game::Rhythm
{
    struct NoteStatus
    {
        int state; // -1: not rendered, 0: rendered but disabled, 1: rendered and visible
        NoteStatus() : state(-1)
        {}
        explicit NoteStatus(const int state) : state(state)
        {}
    };
} // namespace Game::Rhythm