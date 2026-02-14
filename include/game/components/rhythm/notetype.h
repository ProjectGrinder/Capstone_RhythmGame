#pragma once

namespace Game::Rhythm
{
    enum RhythmType
    {
        NORMAL,
        ACCENT,
        RAIN,
    };
    struct NoteType
    {
        RhythmType type;
        NoteType() : type(NORMAL)
        {}
        explicit NoteType(const RhythmType type) : type(type)
        {}
    };
}