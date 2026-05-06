#pragma once

// SUPER CONVENIENT CLASS BOIIII
namespace Math
{
    struct Point
    {
        float x, y, z;

        Point operator+(const Point &other) const
        {
            return { x + other.x, y + other.y, z + other.z};
        }

        Point operator-(const Point &other) const
        {
            return { x - other.x, y - other.y, z - other.z};
        }

        Point operator*(const Point &other) const
        {
            return {x * other.x, y * other.y, z * other.z};
        }

        Point &operator+=(const Point & point)
        {
            this->x += point.x;
            this->y += point.y;
            this->z += point.z;
            return *this;
        }
    };

} // namespace Math

