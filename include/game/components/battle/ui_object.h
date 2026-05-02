#pragma once

namespace Game::Battle
{
    enum UIElements
    {
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
