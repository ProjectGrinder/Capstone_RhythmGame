#pragma once

#include <string>
#include <utility>

namespace Game::Rhythm
{
    struct JudgeText
    {
        std::string text_perfect, text_great, text_fine, text_miss;
        JudgeText(std::string text_perfect, std::string text_great, std::string text_fine, std::string text_miss) :
            text_perfect(std::move(text_perfect)),
            text_great(std::move(text_great)),
            text_fine(std::move(text_fine)),
            text_miss(std::move(text_miss))
        {}
    };
} // namespace Game::Rhythm
