#pragma once
#include "utils/print_debug.h"
#include <array>

namespace Game::Render
{
    enum TweenType
    {
        Linear,
        Ease_In,
        Ease_Out,
        Ease_InOut,
    };

    //TODO: Inputting partial field uses the previous key value or get entity default value.
    // Maybe Add RGBA, ShaderParam
    // In System, check for invalid then assign last field.
    struct AnimationFrame
    {
        Physics::Position position;
        Physics::Rotation rotation;
        Physics::Scale scale;
        int spriteID = -1;
        TweenType tweenType = Linear;
        float tweenStrength = 1.0f;
    };

    //Allow adding only
    struct Animation
    {
        bool is_loop;

        unsigned int currentKey;
        unsigned int keyN;
        unsigned int keyFrameFrame[64];
        AnimationFrame keyframes[64];

        Animation() : is_loop(true), currentKey(0), keyN(0), keyFrameFrame{}, keyframes{}
        {}

        explicit Animation(const unsigned int keyN):
            is_loop(true), currentKey(0), keyN(keyN), keyFrameFrame{}, keyframes{}
        {}
        explicit Animation(const unsigned int keyN, const bool is_loop):
            is_loop(is_loop), currentKey(0), keyN(keyN), keyFrameFrame{}, keyframes{}
        {}
        void AddFrame(const unsigned int frame, AnimationFrame animationFrame)
        {
            keyFrameFrame[currentKey] = frame;
            keyframes[currentKey] = animationFrame;
            currentKey++;
        }
    };

    //TODO: Default state after animation.
    struct Animator
    {
        int animCount;
        std::array<Animation, 32> animations;

        unsigned int frame;
        unsigned int currentKey;
        unsigned int currentAnimation;
        bool is_playing;
        Animator() : animCount(0), frame(0), currentKey(0), currentAnimation(0), is_playing(false)
        {}
        explicit Animator(const unsigned int currentAnimation)
            : animCount(0), frame(0), currentKey(0), currentAnimation(currentAnimation), is_playing(false)
        {}
        void addAnimation(const Animation &animation)
        {
            if (animCount > 32) { LOG_ERROR("Animator Full"); return; }
            animations[animCount++] = animation;
        }

    };
}