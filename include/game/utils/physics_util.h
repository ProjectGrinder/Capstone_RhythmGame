#pragma once
#include <cmath>
#include <random>
#include "maths/point.h"
#include "system/ecs/ecs_types.h"


extern "C" long double get_delta_time();

// TODO : Please find a better place for this
inline float UNASSIGNED = -(1 << 24); // 2^24
inline uint16_t INVALID_ID = UINT16_MAX;
inline System::ECS::pid INVALID_PID = UINT64_MAX;


// Warning : cmath, random
namespace Game::Physics
{
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T clamp(T num, T minN, T maxN)
    {
        num = (num > minN ? num : minN);
        return (num < maxN ? num : maxN);
    }

    inline int sign(const int i)
    {
        if (i)
        {
            return (1);
        }
        return (-1);
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T lerp(T old_x, T new_x, float smoothing)
    {
        return old_x + (new_x - old_x) * smoothing;
    }

    inline Math::Point lerp(const Math::Point old_x, const Math::Point new_x, float smoothing)
    {
        Math::Point result;
        result.x = lerp(old_x.x, new_x.x, smoothing);
        result.y = lerp(old_x.y, new_x.y, smoothing);
        result.z = lerp(old_x.z, new_x.z, smoothing);
        return result;
    }

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    T lerp_at_frame(T old_x, T new_x, int frame)
    {
        const float damping = -log(0.01f) / frame;
        float t = 1.0f - exp(-damping * (float)get_delta_time());
        return old_x + (new_x - old_x) * t;
    }

    inline Math::Point lerp_at_frame(const Math::Point old_x, const Math::Point new_x, int frame)
    {
        Math::Point result;
        result.x = lerp_at_frame(old_x.x, new_x.x, frame);
        result.y = lerp_at_frame(old_x.y, new_x.y, frame);
        result.z = lerp_at_frame(old_x.z, new_x.z, frame);
        return result;
    }

    inline float get_direction(const float x1, const float y1, const float x2, const float y2)
    {
        return (atan2(y2 - y1, x2 - x1));
    }

    inline float get_direction(const Math::Point p1, const Math::Point p2)
    {
        return (atan2(p2.y - p1.y, p2.x - p1.x));
    }


    inline float get_distance(const float x1, const float y1, const float x2, const float y2)
    {
        return (static_cast<float>(pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5)));
    }

    inline float get_distance(const Math::Point p1, const Math::Point p2)
    {
        return (static_cast<float>(pow(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2), 0.5)));
    }

    inline float rand_float(const float min, const float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution dist(min, max);
        return (dist(gen));
    }

    inline int rand_int(const int min, const int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution dist(min, max);
        return (dist(gen));
    }
} // namespace Game::Physics
