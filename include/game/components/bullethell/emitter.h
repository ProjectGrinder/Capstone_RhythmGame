#pragma once

#include "particle.h"

// TODO : Each of them should be inputted with prototype entity (prefab) + Single Bullet spawning is currently being
// considered
namespace Game::BulletHell
{
    struct BoomEmitter
    {
        Bullet particle;
        float delay;
        float scale;
        float sustain_time;
        explicit BoomEmitter(const Bullet particle, const float delay, const float scale, const float sustain_time) :
            particle(particle), delay(delay), scale(scale), sustain_time(sustain_time)
        {}
    };

    struct LaserEmitter
    {
        Bullet particle;
        float delay;
        float rotation;
        float width;
        float length;
        float sustain_time;
        float speed;
        explicit LaserEmitter(
                const Bullet particle,
                const float delay,
                const float rotation,
                const float width,
                const float length,
                const float sustain_time,
                const float speed) :
            particle(particle),
            delay(delay),
            rotation(rotation),
            width(width),
            length(length),
            sustain_time(sustain_time),
            speed(speed)
        {}
    };

    struct RingEmitter
    {
        Particle particle;
        float delay;
        float rotation;
        float width;
        float length;
        float fire_delay;
        int bullet_quantity;

        int bullet_fired = 0;
        explicit RingEmitter(
                const Particle particle,
                const float delay,
                const float rotation,
                const float width,
                const float length,
                const float fire_delay,
                const int bullet_quantity) :
            particle(particle),
            delay(delay),
            rotation(rotation),
            width(width),
            length(length),
            fire_delay(fire_delay),
            bullet_quantity(bullet_quantity)
        {}
    };

    struct RandomEmitter
    {
        Particle particle;
        float delay;
        float angle_range;
        float center_rotation;
        float width;
        float length;
        float fire_delay;
        int bullet_quantity;
        explicit RandomEmitter(
                const Particle particle,
                const float delay,
                const float angle_range,
                const float center_rotation,
                const float width,
                const float length,
                const float fire_delay,
                const int bullet_quantity) :
            particle(particle),
            delay(delay),
            angle_range(angle_range),
            center_rotation(center_rotation),
            width(width),
            length(length),
            fire_delay(fire_delay),
            bullet_quantity(bullet_quantity)
        {}
    };

    struct SpreadEmitter
    {
        Particle particle;
        float delay;
        float angle_range;
        float center_rotation;
        float width;
        float length;
        float fire_delay;
        int bullet_quantity;

        int bullet_fired = 0;
        explicit SpreadEmitter(
                const Particle particle,
                const float delay,
                const float angle_range,
                const float center_rotation,
                const float width,
                const float length,
                const float fire_delay,
                const int bullet_quantity) :
            particle(particle),
            delay(delay),
            angle_range(angle_range),
            center_rotation(center_rotation),
            width(width),
            length(length),
            fire_delay(fire_delay),
            bullet_quantity(bullet_quantity)
        {}
    };
} // namespace Game::BulletHell
