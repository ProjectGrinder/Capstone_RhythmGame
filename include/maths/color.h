#pragma once
namespace Math
{
    struct Color
    {
        float r, g, b, a;
        Color(const float r = 1, const float g = 1, const float b = 1, const float a = 1) : r{r}, g{g}, b{b}, a{a}{}
    };
} // namespace Math
