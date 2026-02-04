#pragma once
#include <concepts>
#include <stdexcept>

template<typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

namespace Math
{
    template<arithmetic T>
    struct Vector2
    {
        T x;
        T y;
        Vector2() : x(0), y(0)
        {}
        Vector2(T x, T y) : x(x), y(y)
        {}
        Vector2(const Vector2 &other) : x(other.x), y(other.y)
        {}
        Vector2 operator+(const Vector2 &other) const
        {
            return (Vector2(x + other.x, y + other.y));
        }
        Vector2 operator-(const Vector2 &other) const
        {
            return (Vector2(x - other.x, y - other.y));
        }
        Vector2 operator*(T scalar) const
        {
            return (Vector2(x * scalar, y * scalar));
        }
        Vector2 operator/(T scalar) const
        {
            if (scalar != 0)
            {
                return (Vector2(x / scalar, y / scalar));
            }
            else
            {
                throw(std::runtime_error("Division by zero"));
            }
        }

        Vector2 &operator=(const Vector2 &other)
        {
            if (this != &other)
            {
                x = other.x;
                y = other.y;
            }
            return (*this);
        }

        bool operator==(const Vector2 &other) const
        {
            return (x == other.x && y == other.y);
        }
    };
} // namespace Math
