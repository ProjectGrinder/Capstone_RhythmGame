#pragma once

#include "particle.h"

//TODO : Each of them should be inputted with prototype entity (prefab) + Single Bullet spawning is currently being considered
namespace Game::BulletHell
{
    struct BoomEmitter
    {
        Particle particle;
        float rotation;
        float scale;
        float sustain_time;
        explicit BoomEmitter(const Particle particle, const float rotation, const float scale, const float sustain_time) :
            particle(particle), rotation(rotation), scale(scale), sustain_time(sustain_time)
        {}
    };

    struct LaserEmitter
    {
        Particle particle;
        float rotation;
        float width;
        float length;
        float sustain_time;
        float speed;
        explicit LaserEmitter(const Particle particle, const float rotation, const float width, const float length, const float sustain_time, const float speed) :
            particle(particle), rotation(rotation), width(width), length(length), sustain_time(sustain_time), speed(speed)
        {}
    };

    struct RingEmitter
    {
        Particle particle;
        float rotation;
        float width;
        float length;
        float fire_delay;
        int bullet_quantity;
        explicit RingEmitter(const Particle particle, const float rotation, const float width, const float length, const float fire_delay, const int bullet_quantity) :
            particle(particle), rotation(rotation), width(width), length(length), fire_delay(fire_delay), bullet_quantity(bullet_quantity)
        {}
    };

    struct RandomEmitter
    {
        Particle particle;
        float angle_range;
        float center_rotation;
        float width;
        float length;
        float fire_delay;
        int bullet_quantity;
        explicit RandomEmitter(const Particle particle, const float angle_range, const float center_rotation, const float width, const float length, const float fire_delay, const int bullet_quantity) :
            particle(particle), angle_range(angle_range), center_rotation(center_rotation), width(width), length(length), fire_delay(fire_delay), bullet_quantity(bullet_quantity)
        {}
    };

    struct SpreadEmitter
    {
        Particle particle;
        float angle_range;
        float center_rotation;
        float width;
        float length;
        int bullet_quantity;
        explicit SpreadEmitter(const Particle particle, const float angle_range, const float center_rotation, const float width, const float length, const int bullet_quantity) :
            particle(particle), angle_range(angle_range), center_rotation(center_rotation), width(width), length(length), bullet_quantity(bullet_quantity)
        {}
    };
}