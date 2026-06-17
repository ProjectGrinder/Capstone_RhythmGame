#pragma once
#include <array>
#include <map>

namespace Game::Render
{
    struct AnimationData
    {
        Math::Point size;
        int offset_y;
        int frame;
        bool isLoop;
        int fps;
        int loop_frame;
        AnimationData(
                const Math::Point size,
                const int offset_y,
                const int frame,
                const int fps = 60,
                const bool isLoop = true,
                const int loop_frame = 0) :
            size(size), offset_y(offset_y), frame(frame), isLoop(isLoop), fps(fps), loop_frame(loop_frame) {
            };
    };

    // Map?
    struct AnimationDataRegistry
    {
        std::unordered_map<std::string, AnimationData> animation_datas;
        AnimationDataRegistry(std::unordered_map<std::string,AnimationData> data)
            : animation_datas(std::move(data)) {}
    };
} // namespace Game::Render
