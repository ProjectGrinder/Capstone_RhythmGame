#pragma once

namespace Game::Battle
{
    enum UIElements
    {
        PhaseChangeText, // Text for now
        HpBar,
        HPBarMax,
        GrazeText,
        ScoreText
    };

    struct UIComponent
    {
        UIElements type;
    };
} // namespace Game::Battle
