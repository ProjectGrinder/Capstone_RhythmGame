#pragma once

#include "game/components.h"

extern "C" long double get_delta_time();

namespace Game::Rhythm
{
    template<typename T>
    void update_judge_text(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<JudgeText, Render::Text> &query)
    {
        if (query.begin() == query.end())
            return;

        auto comp = query.front();

        if (!comp.get<JudgeText>().change)
        {
            if (comp.get<JudgeText>().timer <= 0)
                return;

            comp.get<JudgeText>().timer -= static_cast<int>(get_delta_time() * 1000);
            if (comp.get<JudgeText>().timer <= 0)
            {
                comp.get<Render::Text>().text = "";
            }
            return;
        }

        switch (comp.get<JudgeText>().judge)
        {
        case JudgeText::PERFECT :
            comp.get<Render::Text>().text = "PERFECT";
            comp.get<Render::Text>().color = {1, 1, 0, 1};
            break;

        case JudgeText::GREAT :
            comp.get<Render::Text>().text = "GREAT";
            comp.get<Render::Text>().color = {0, 1, 0.5, 1};
            break;

        case JudgeText::FINE :
            comp.get<Render::Text>().text = "FINE";
            comp.get<Render::Text>().color = {0.5, 1, 1, 1};
            break;

        case JudgeText::MISS :
            comp.get<Render::Text>().text = "MISS";
            comp.get<Render::Text>().color = {1, 0, 0, 1};
            break;

        default:
            comp.get<Render::Text>().text = "";
            break;
        }
        comp.get<JudgeText>().timer = 1000000;
        comp.get<JudgeText>().change = false;
    }
}
