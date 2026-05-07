#pragma once
#include "system/intent_storage.h"

namespace Game::Battle
{
    template<typename T>
    void phase_change(
        [[maybe_unused]] T &syscall,
        System::ECS::Query<TransitionData> &transition_query,
        System::ECS::Query<BattleState> &battle_query,
        System::ECS::Query<Audio::SoundRegistry> &sound_query,
        System::ECS::Query<UIComponent, Render::Text, Render::Material, Render::Transform> &transition_text_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        auto &battle_state = battle_query.front().get<BattleState>();
        auto &transition_text = transition_text_query.front().get<Render::Text>();


        // Init Music. Fix later
        if (battle_state.clock_time>=0 && battle_state.clock_time/1000<1000)
        {
            if (!sound_query.front().get<Audio::SoundRegistry>().audios["DemoSong"].play)
            {
                Audio::audio_play(sound_query.front().get<Audio::SoundRegistry>().audios["DemoSong"]);
                sound_query.front().get<Audio::SoundRegistry>().audios["DemoSong"].play = true;
            }
        }

        for (auto &[id, comps] : transition_query)
        {
            auto &transition_data = comps.get<TransitionData>();
            if (transition_data.timing_start > battle_state.clock_time/1000)
            {
                continue;
            }

            // Changing state
            if (transition_data.state == 4 && transition_data.timing_start < battle_state.clock_time/1000)
            {
                transition_text_query.front().get<Render::Material>().visible = true;
                transition_text.text = "3";
                transition_text.color = Math::Color{0.2f,1,0.2f,1};
                transition_data.state = 3;
            }
            else if (transition_data.state == 3 && transition_data.timing_start + transition_data.duration/3 < battle_state.clock_time/1000)
            {
                transition_text.text = "2";
                transition_text.color = Math::Color{0.75f,0.75f,0.2f,1};
                transition_data.state = 2;
            }
            else if (transition_data.state == 2 && transition_data.timing_start + transition_data.duration*2/3 < battle_state.clock_time/1000)
            {
                transition_text.text = "1";
                transition_text.color = Math::Color{1,0.2f,0.2f,1};
                transition_data.state = 1;
                battle_state.current_phase = transition_data.phase;
            }
            else if (transition_data.state == 1 && transition_data.timing_start + transition_data.duration < battle_state.clock_time/1000)
            {
                if (transition_data.phase == BULLET_HELL)
                {
                    // No Set text center option
                    transition_text_query.front().get<Render::Transform>().position.x = -50;
                    transition_text.text = "Call";
                }
                else if (transition_data.phase == RHYTHM)
                {
                    transition_text_query.front().get<Render::Transform>().position.x = -100;
                    transition_text.text = "Respond";
                }
                transition_text.color = Math::Color{1,0.2f,0.2f,1};
                // battle_state.current_phase = transition_data.phase;
                transition_data.state = 0;

            }
            else if (transition_data.state == 0 && transition_data.timing_start + transition_data.duration*5/3 < battle_state.clock_time/1000)
            {
                transition_text_query.front().get<Render::Transform>().position.x = 0;
                transition_text_query.front().get<Render::Material>().visible = false;
                transition_text.text = "";
                syscall.remove_entity(id);
            }
        }


    }
} // namespace Game::Battle
