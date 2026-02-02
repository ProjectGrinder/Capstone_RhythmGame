#pragma once
#include <cmath>
#include <random>

#include "game/components/physics/position.h"


// TODO : Please find a better place for this
constexpr float UNASSIGNED = -(1 << 24); // 2^24

// Warning : cmath, random
namespace Game::Physics
{


    template<typename T>
    T clamp(T num, T minN, T maxN)
    {
        num = (num > minN ? num : minN);
        return num < maxN ? num : maxN;
    }

    inline int sign(const int i){
        if(i) { return 1; }
        return -1;
    }


    inline float get_direction(const float x1, const float y1, const float x2, const float y2){
        return atan2(y2 - y1,x2 - x1);
    }

    inline float get_direction(const Position p1,const Position p2){
        return atan2(p2.y - p1.y,p2.x - p1.x);
    }


    inline float get_distance(const float x1,const float y1,const float x2,const float y2){
        return static_cast<float>(pow(pow(x2-x1,2)+pow(y2-y1,2),0.5));
    }

    inline float get_distance(const Position p1,const Position p2){
        return static_cast<float>(pow(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2),0.5));
    }

    inline float rand_float(const float min, const float max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution dist(min, max);
        return dist(gen);
    }

    inline int rand_int(const int min, const int max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution dist(min, max);
        return dist(gen);
    }
}