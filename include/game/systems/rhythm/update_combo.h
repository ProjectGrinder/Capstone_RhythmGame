#pragma once

#include "game/components.h"

namespace Game::Rhythm
{
    template<typename T>
    void update_combo(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<Combo, Render::Text> &combo_query)
    {
        if (combo_query.begin() == combo_query.end())
            return;

        auto comp = combo_query.front();
        auto combo = comp.get<Combo>().combo;

        if (combo > 0)
        {
            comp.get<Render::Text>().text = std::to_string(combo);
        }
        else
        {
            comp.get<Render::Text>().text = "";
        }
    }
}