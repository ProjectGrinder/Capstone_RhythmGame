#pragma once
namespace Math
{
    struct Point
    {
        float x, y, z;

        Point operator*(const Point &other) const
        {
            return { x * other.x, y * other.y, z * other.z};
        }
    };

} // namespace Math

