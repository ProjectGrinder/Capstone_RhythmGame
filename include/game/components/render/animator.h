#pragma once
#include <array>

namespace Game::Render
{
    struct Animator
    {
        std::string anim_name;
        // bool isLoop = false;
        int current_frame = 0;
        int current_time = 0;

        Animator(
                const std::string &anim_name = ""): anim_name(anim_name)
        {}
    };

    struct AnimationSequence
    {
        std::vector<std::pair<int, std::string>> animation_seq;
        int current_anim = 0;
        int current_time = 0;
        bool isLoop;
        int loop_frame;
        int loop_start_frame;
        AnimationSequence(const std::vector<std::pair<int, std::string>> &animation_seq = {}, const bool isLoop = false, const int loop_frame = -1, const int loop_start = 0) :
            animation_seq(animation_seq),  isLoop(isLoop), loop_start_frame(loop_start)
        {
            if (loop_frame < 0 && !animation_seq.empty()) this->loop_frame = animation_seq[animation_seq.size() - 1].first;
        };
    };

    struct Animation_Controller
    {
        std::string to_anim = "";
        bool has_exit_time;
        int transition_time;

        Animation_Controller(bool has_exit_time = false) : has_exit_time(has_exit_time),transition_time(0){}
    };
} // namespace Game::Render
