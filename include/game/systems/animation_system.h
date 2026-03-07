#pragma once

#include <cmath>
#include "game/components.h"

// TODO: Check frame_time
//  Problem that I can't implement this further -> Handling object with partial components
namespace Game::Render
{
    inline float tween_transform(float t, float a, float b, TweenType tween, float strength);

    template<typename T>
    void animation_system(
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
            const auto &animation = animator.animations[animator.currentAnimation];
            auto &sprite = comps.get<Sprite>();

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
                sprite.id = animation.keyframes[currentKey].spriteID;
            }

            const auto &keyFrameFrame1 = animation.keyFrameFrame[currentKey];
            const auto &keyFrameFrame2 = animation.keyFrameFrame[currentKey + 1];
            const auto &keyFrame1 = animation.keyframes[currentKey];
            const auto &keyFrame2 = animation.keyframes[currentKey + 1];

            auto &position = comps.get<Physics::Position>();
            auto &rotation = comps.get<Rotation>();
            auto &scale = comps.get<Physics::Scale>();

            const float t =
                    static_cast<float>(frame - keyFrameFrame1) / static_cast<float>(keyFrameFrame2 - keyFrameFrame1);

            position.x = tween_transform(
                    t, keyFrame1.position.x, keyFrame2.position.x, keyFrame1.tweenType, keyFrame1.tweenStrength);
            position.y = tween_transform(
                    t, keyFrame1.position.y, keyFrame2.position.y, keyFrame1.tweenType, keyFrame1.tweenStrength);
            position.z = tween_transform(
                    t, keyFrame1.position.z, keyFrame2.position.z, keyFrame1.tweenType, keyFrame1.tweenStrength);
            rotation.angleX = tween_transform(
                    t,
                    keyFrame1.rotation.angleX,
                    keyFrame2.rotation.angleX,
                    keyFrame1.tweenType,
                    keyFrame1.tweenStrength);
            rotation.angleY = tween_transform(
                    t,
                    keyFrame1.rotation.angleY,
                    keyFrame2.rotation.angleY,
                    keyFrame1.tweenType,
                    keyFrame1.tweenStrength);
            rotation.angleZ = tween_transform(
                    t,
                    keyFrame1.rotation.angleZ,
                    keyFrame2.rotation.angleZ,
                    keyFrame1.tweenType,
                    keyFrame1.tweenStrength);
            scale.scaleX = tween_transform(
                    t, keyFrame1.scale.scaleX, keyFrame2.scale.scaleX, keyFrame1.tweenType, keyFrame1.tweenStrength);
            scale.scaleY = tween_transform(
                    t, keyFrame1.scale.scaleY, keyFrame2.scale.scaleY, keyFrame1.tweenType, keyFrame1.tweenStrength);
            scale.scaleZ = tween_transform(
                    t, keyFrame1.scale.scaleZ, keyFrame2.scale.scaleZ, keyFrame1.tweenType, keyFrame1.tweenStrength);
        }
    }

    inline float
    tween_transform(const float t, const float a, const float b, const TweenType tween, const float strength)
    {
        const float v = b - a;
        switch (tween)
        {
        case Linear:
            return a + v * t;
        case Ease_In:
            return a - v * pow(t, strength);
        case Ease_Out:
            return a + v * (1 - pow(1 - t, strength));
        case Ease_InOut:
            if (t < 0.5)
                return a + v * 0.5f * pow(2 * t, strength);
            return a + v * (1 - 0.5f * pow(2 * (1 - t), strength));

        default:
            return a + v * t;
        }
    }
} // namespace Game::Render
