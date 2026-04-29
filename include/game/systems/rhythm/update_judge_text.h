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
        auto comp = query.front();
        std::string text;



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
            text = "PERFECT";
            break;

        case JudgeText::GREAT :
            text = "GREAT";
            break;

        case JudgeText::FINE :
            text = "FINE";
            break;

        case JudgeText::MISS :
            text = "MISS";
            break;

        default:
            text = "";
            break;
        }

        comp.get<Render::Text>().text = text;
        comp.get<JudgeText>().timer = 1000000;
        comp.get<JudgeText>().change = false;
    }
}
