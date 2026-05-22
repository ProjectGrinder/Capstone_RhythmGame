#pragma once

namespace Game::Rhythm
{
    struct NoteType
    {
        int type;
        NoteType() : type(0) // 0 - normal, 1 - accent, 2 - rain | -1 - hold end
        {}
        explicit NoteType(const int type) : type(type)
        {}
    };
}