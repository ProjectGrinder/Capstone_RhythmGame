#pragma once

namespace Game::Render
{
    //TODO: Default state after animation.
    struct Animator
    {
        int animCount;
        Animation animations[32];

        unsigned int frame;
        unsigned int currentKey;
        unsigned int currentAnimation;
        bool is_playing;
        Animator() : animCount(0), animations{}, frame(0), currentKey(0), currentAnimation(0), is_playing(false)
        {}
        explicit Animator(const unsigned int currentAnimation)
            : animCount(0), animations{}, frame(0), currentKey(0), currentAnimation(currentAnimation), is_playing(false)
        {}
        void addAnimation(const Animation &animation)
        {
            if (animCount > 32) { LOG_ERROR("Animator Full"); return; }
            animations[animCount++] = animation;
        }

    };
}