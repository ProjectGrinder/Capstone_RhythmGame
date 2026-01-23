#pragma once

// TODO: Marked by Nack
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
    struct AnimationFrame
    {
        Physics::Position position;
        Physics::Rotation rotation;
        Physics::Scale scale;
        int spriteID = -1;
        TweenType tweenType = Linear;
        float tweenStrength = 1.0f;
    };

    //Not Component
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


    /*struct Animation
    {
        AnimationFrame keyframes[32];
        template <typename ... Key>
        explicit Animation(Key ...keys) : keyframes{}
        {
            constexpr int n = sizeof...(Key);
            const AnimationFrame tmp[n] = { static_cast<AnimationFrame>(keys)...};
            std::memcpy(keyframes, tmp, sizeof(tmp));
            if constexpr (n < 32)
            {
                for (int i = n; i < 32; ++i)
                {
                    keyframes[i] = {-1};
                }
            }
        }
    };*/
}