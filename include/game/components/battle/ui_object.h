#pragma once

namespace Game::Battle
{
    enum UIElements
    {
        HpBar,
        HPBarMax,
        AcceptBar,
        AcceptBarMax,
        GrazeText,
        AccuracyText,
        StatBox,
        SongTitle,
        ArtistName,
        LevelDiff
    };

    struct UIComponent
    {
        UIElements type;
    };
} // namespace Game::Battle
