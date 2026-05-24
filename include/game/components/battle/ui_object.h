#pragma once

namespace Game::Battle
{
    enum UIElements
    {
        HpBar,
        HPBarMax,
        GrazeText,
        AccuracyText
    };

    struct UIComponent
    {
        UIElements type;
    };
} // namespace Game::Battle
