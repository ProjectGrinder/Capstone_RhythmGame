#pragma once
#include "game/components/render/animation_data.h"
#include "game/components/render/animator.h"
#include "game/components/render/sprite.h"
#include "system.h"

namespace Game::Render
{
    // TODO Later : anim state / anim_transition
    template<typename T>
    void animation_system([[maybe_unused]] T &syscall,
        System::ECS::Query<AnimationDataRegistry> &anim_data_query,
        System::ECS::Query<Sprite, Animator> &query)
    {
        if (anim_data_query.begin() == anim_data_query.end())
            return;
        const auto &anim_reg = anim_data_query.front().get<AnimationDataRegistry>();

        for (auto &[id, comp]: query)
        {
            auto &sprite = comp.get<Sprite>();
            auto &animator = comp.get<Animator>();
            if (animator.anim_name == "") continue;

            const auto &anim_data = anim_reg.animation_datas.at(animator.anim_name);
            if (animator.current_time >= 1000/anim_data.fps)
            {
                animator.current_frame++;
                if (animator.current_frame >= anim_data.frame)
                {
                    if (anim_data.isLoop)
                        animator.current_frame = anim_data.loop_frame;
                    else
                        animator.current_frame = anim_data.frame-1;

                }

                sprite.u0 = anim_data.size.x * animator.current_frame;
                sprite.u1 = anim_data.size.x * (animator.current_frame+1);

                animator.current_time = 0;
            }

            animator.current_time+=(int)get_delta_time();
        }
    }

    template<typename T>
    void animation_sequence_system([[maybe_unused]] T &syscall,
        System::ECS::Query<Animation_Controller, AnimationSequence> &query)
    {
        for (auto &[id, comp]: query)
        {
            auto &anim_seq = comp.get<AnimationSequence>();
            auto &anim_con = comp.get<Animation_Controller>();

            anim_seq.current_time += static_cast<int>(get_delta_time());
            if (anim_seq.current_anim != anim_seq.animation_seq.size()-1 && anim_seq.current_time >= anim_seq.animation_seq[anim_seq.current_anim+1].first)
                anim_con.to_anim = anim_seq.animation_seq[++anim_seq.current_anim].second;


            if (anim_seq.isLoop)
            {
                if (anim_seq.current_time >= anim_seq.loop_frame)
                {
                    anim_seq.current_time = anim_seq.loop_start_frame;
                    anim_seq.current_anim = 0;
                    while (anim_seq.current_anim < anim_seq.animation_seq.size()-1 && anim_seq.current_time >= anim_seq.animation_seq[anim_seq.current_anim+1].first)
                        anim_seq.current_anim++;
                    anim_con.to_anim = anim_seq.animation_seq[anim_seq.current_anim].second;
                }
            }
        }
    }

    template<typename T>
    void anim_transition_system([[maybe_unused]] T &syscall,
        System::ECS::Query<AnimationDataRegistry> &anim_data_query,
        System::ECS::Query<Sprite, Animator, Animation_Controller> &query)
    {
        if (anim_data_query.begin() == anim_data_query.end())
            return;
        const auto &anim_reg = anim_data_query.front().get<AnimationDataRegistry>();

        for (auto &[id, comp]: query)
        {
            auto &sprite = comp.get<Sprite>();
            auto &animator = comp.get<Animator>();
            auto &anim_con = comp.get<Animation_Controller>();

            if (anim_con.to_anim != animator.anim_name)
            {
                if ((!anim_con.has_exit_time ||
                    (animator.anim_name != "" && animator.current_frame == anim_reg.animation_datas.at(animator.anim_name).frame-1)) &&
                    anim_con.to_anim != "")
                {
                    const auto &anim_data = anim_reg.animation_datas.at(anim_con.to_anim);
                    sprite.u0 = 0;
                    sprite.u1 = anim_data.size.x;
                    sprite.v0 = anim_data.offset_y * anim_data.size.y;
                    sprite.v1 = anim_data.offset_y * anim_data.size.y + anim_data.size.y;
                    animator.current_time = 0;
                    animator.current_frame = 0;
                    animator.anim_name = anim_con.to_anim;
                }
            }
        }
    }
} // namespace Game::Render
