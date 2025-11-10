#pragma once

namespace Game::Rhythm
{
    struct NoteSpeed
    {
        float speed;
        explicit NoteSpeed(const float speed) : speed(speed)
        {}
    };
}