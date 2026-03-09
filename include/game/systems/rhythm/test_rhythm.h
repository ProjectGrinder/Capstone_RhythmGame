#pragma once
#include "game/components.h"
#include "utils/print_debug.h"

namespace Game::Rhythm
{
    inline void check_judgement(
        const int note_timing,
        const int current_timing)
    {
        const auto time_diff = current_timing - note_timing;
        constexpr auto perfect_judge = 500;
        constexpr auto great_judge = 750;
        constexpr auto fine_judge = 1000;
        if (time_diff > -1 * perfect_judge && time_diff < perfect_judge)
        {
            LOG_INFO("Timing %d: Perfect (Diff: %d ms)", note_timing, time_diff);
        }
        else if (time_diff > -1 * great_judge && time_diff < great_judge)
        {
            LOG_INFO("Timing %d: Great (Diff: %d ms)", note_timing, time_diff);
        }
        else if (time_diff > -1 * fine_judge && time_diff < fine_judge)
        {
            LOG_INFO("Timing %d: Fine (Diff: %d ms)", note_timing, time_diff);
        }
        else
        {
            LOG_INFO("Not in range! Please wait...")
        }
    }
    template<typename T>
    void test_rhythm(
        [[maybe_unused]] const T& syscall,
        System::ECS::Query<Battle::BattleState> &battle_query,
        System::ECS::Query<KeyInput> &input_query)
    {
        if (battle_query.begin() == battle_query.end())
            return;

        if (input_query.begin() == input_query.end())
            return;

        //Check only key 2 (F)
        if (input_query.front().get<KeyInput>().key2_pressed)
        {
            auto current_timing = battle_query.front().get<Battle::BattleState>().clock_time / 1000;
            if (current_timing < 6000)
            {
                check_judgement(5000, current_timing);
            }
            else if (current_timing < 11000)
            {
                check_judgement(10000, current_timing);
            }
            else if (current_timing < 16000)
            {
                check_judgement(15000, current_timing);
            }
            else if (current_timing < 21000)
            {
                check_judgement(20000, current_timing);
            }
            else if (current_timing < 26000)
            {
                check_judgement(25000, current_timing);
            }
        }
    }
}
