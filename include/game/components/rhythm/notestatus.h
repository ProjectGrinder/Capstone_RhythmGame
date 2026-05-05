#pragma once

namespace Game::Rhythm
{
    struct NoteStatus
    {
        int state;
        // 0 - not rendered yet
        // 1 - rendered and visible
        // -1 - discarded (hit/miss)
        NoteStatus() : state(0)
        {}
        explicit NoteStatus(const int state) : state(state)
        {}
    };
} // namespace Game::Rhythm