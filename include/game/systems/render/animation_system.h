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
        const auto &anim_reg = anim_data_query.front().get<AnimationDataRegistry>();

        for (auto &[id, comp]: query)
        {
            auto &sprite = comp.get<Sprite>();
            auto &animator = comp.get<Animator>();
            LOG_INFO("%d",animator.anim_id);
            if (animator.anim_id == -1) continue;

            const auto &anim_data = anim_reg.animation_datas.at(animator.anim_id);
            if (animator.current_time >= 1000/anim_data.fps)
            {
                animator.current_frame++;
                if (animator.current_frame >= anim_data.frame)
                {
                    if (anim_data.isLoop)
                        animator.current_frame = anim_data.loop_frame;
                    else
                        continue;
                }

                sprite.u0 = anim_data.size.x * animator.current_frame;
                sprite.u1 = anim_data.size.x * (animator.current_frame+1);

                animator.current_time = 0;
            }

            animator.current_time+=(int)get_delta_time();
        }
    }

    template<typename T>
    void anim_transition_system([[maybe_unused]] T &syscall,
        System::ECS::Query<AnimationDataRegistry> &anim_data_query,
        System::ECS::Query<Sprite, Animator, Animation_Controller> &query)
    {
        const auto &anim_reg = anim_data_query.front().get<AnimationDataRegistry>();

        for (auto &[id, comp]: query)
        {
            auto &sprite = comp.get<Sprite>();
            auto &animator = comp.get<Animator>();
            auto &anim_con = comp.get<Animation_Controller>();

            anim_con.current_id = animator.anim_id;

            if (anim_con.to_id != anim_con.current_id)
            {
                if (!anim_con.has_exit_time || animator.current_frame != 0 && anim_con.to_id != -1)
                {
                    const auto &anim_data = anim_reg.animation_datas.at(anim_con.to_id);
                    sprite.v0 = anim_data.offset_y * anim_data.size.y;
                    sprite.v1 = anim_data.offset_y * anim_data.size.y + anim_data.size.y;
                    animator.current_time = 0;
                    animator.current_frame = 0;
                    animator.anim_id = anim_con.to_id;
                }
            }
        }
    }
} // namespace Game::Render
