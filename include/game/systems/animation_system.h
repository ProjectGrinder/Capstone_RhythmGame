#pragma once

#include <cmath>
#include "game/components.h"

// TODO: Check frame_time
//  Problem that I can't implement this further -> Handling object with partial components
namespace Game::Render
{
    inline void InitializeAnim(Animation& animation, char* texture);
    inline float TweenTransform(float t, float a, float b, TweenType tween, float strength);

    template<typename T>
    void AnimationSystem(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Physics::Position, Rotation, Physics::Scale, Sprite, Animator> &query)
    {

        for (auto &[id, comps]: query)
        {
            auto &animator = comps.get<Animator>();
            if (!animator.is_playing)
            {
                continue;
            }

            const auto &frame = animator.frame;
            auto &currentKey = animator.currentKey;
            auto &animation = animator.animations[animator.currentAnimation];
            auto &sprite = comps.get<Sprite>();

            if (!animation.isInitialized)
            {
                InitializeAnim(animation, sprite.texture);
                animation.isInitialized = true;
            }

            // Check Next Key
            if (animation.keyFrameFrame[currentKey + 1] <= frame)
            {
                if (currentKey == animation.keyN)
                {
                    // TODO: Return Default State
                    if (!animation.is_loop)
                        animator.is_playing = false;
                    currentKey = 0;
                    continue;
                }
                currentKey++;

                if (animation.keyframes[currentKey].texture != nullptr)
                    sprite.texture = animation.keyframes[currentKey].texture;
            }

            const auto &keyFrameFrame1 = animation.keyFrameFrame[currentKey];
            const auto &keyFrameFrame2 = animation.keyFrameFrame[currentKey + 1];
            const auto &keyFrame1 = animation.keyframes[currentKey];
            const auto &keyFrame2 = animation.keyframes[currentKey + 1];

            auto &position = comps.get<Physics::Position>();
            auto &rotation = comps.get<Rotation>();
            auto &scale = comps.get<Physics::Scale>();

            const float t = static_cast<float>(frame - keyFrameFrame1) / static_cast<float>(keyFrameFrame2 - keyFrameFrame1);

            position.x = TweenTransform(t,keyFrame1.posX,keyFrame2.posY,keyFrame1.tweenType,keyFrame1.tweenStrength);
            position.y = TweenTransform(t,keyFrame1.posY,keyFrame2.posY,keyFrame1.tweenType,keyFrame1.tweenStrength);
            position.z = TweenTransform(t,keyFrame1.posZ,keyFrame2.posZ,keyFrame1.tweenType,keyFrame1.tweenStrength);
            rotation.angleX = TweenTransform(t,keyFrame1.rotX,keyFrame2.rotX,keyFrame1.tweenType,keyFrame1.tweenStrength);
            rotation.angleY = TweenTransform(t,keyFrame1.rotY,keyFrame2.rotY,keyFrame1.tweenType,keyFrame1.tweenStrength);
            rotation.angleZ = TweenTransform(t,keyFrame1.rotZ,keyFrame2.rotZ,keyFrame1.tweenType,keyFrame1.tweenStrength);
            scale.scaleX = TweenTransform(t,keyFrame1.sclX,keyFrame2.sclX,keyFrame1.tweenType,keyFrame1.tweenStrength);
            scale.scaleY = TweenTransform(t,keyFrame1.sclY,keyFrame2.sclY,keyFrame1.tweenType,keyFrame1.tweenStrength);
            scale.scaleZ = TweenTransform(t,keyFrame1.sclZ,keyFrame2.sclZ,keyFrame1.tweenType,keyFrame1.tweenStrength);

        }
    }

    inline float TweenTransform(const float t, const float a, const float b, const TweenType tween, const float strength)
    {
        const float v = b-a;
        switch (tween)
        {
            case Linear:
                return a + v * t;
            case Ease_In:
                return a - v * pow(t,strength);
            case Ease_Out:
                return a + v * (1 - pow(1-t,strength));
            case Ease_InOut:
                if (t < 0.5)
                    return a + v * 0.5f * pow(2*t,strength);
                return a + v * (1 - 0.5f * pow(2*(1 - t), strength));

            default: return a + v * t;
        }
    }

    inline void InitializeAnim(Animation& animation, char* texture)
    {
        AnimationFrame prev{texture};

        for (auto& frame : animation.keyframes)
        {
            if (frame.posX == UNASSIGNED) frame.posX = prev.posX;
            if (frame.posY == UNASSIGNED) frame.posY = prev.posY;
            if (frame.posZ == UNASSIGNED) frame.posZ = prev.posZ;
            if (frame.rotX == UNASSIGNED) frame.rotX = prev.rotX;
            if (frame.rotY == UNASSIGNED) frame.rotY = prev.rotY;
            if (frame.rotZ == UNASSIGNED) frame.rotZ = prev.rotZ;
            if (frame.sclX == UNASSIGNED) frame.sclX = prev.sclX;
            if (frame.sclY == UNASSIGNED) frame.sclY = prev.sclY;
            if (frame.sclZ == UNASSIGNED) frame.sclZ = prev.sclZ;
            if (frame.texture == nullptr) frame.texture = prev.texture;


            prev = frame;
        }
    }
}