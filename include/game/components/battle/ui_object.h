#pragma once

namespace Game::Battle
{
    enum UIElements
    {
        HpBar,
        HPBarMax,
        GrazeText,
        AccuracyText,
        StatBox,
        LevelDiff
    };

    struct UIComponent
    {
        UIElements type;
    };
} // namespace Game::Battle
