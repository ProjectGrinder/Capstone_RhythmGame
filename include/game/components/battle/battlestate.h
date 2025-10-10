#pragma once
#include <vector>
#include <string>

namespace Game::Battle
{
    enum Instrument
    {
        NO_INSTRUMENT,
    };

    struct BulletHellState
    {
        int graze;
        float iframe_time;
    };

    struct RhythmState
    {
        int heal_hp;
        int base_score;
        int total_notes;
        float note_speed;
    };

    struct Acceptance
    {
        int total_accept;
        int current_accept;
        int max_accept_gauge;
    };

    struct NoteData
    {
        bool is_hold;
        float timing;
        float timing_end;
    };

    struct LaneInfo
    {
        int lane_number;
        std::vector<NoteData> notes;
        unsigned int current_note;
    };

    struct ChartData
    {
        LaneInfo lanes[4];
    };

    struct BpmInfo
    {
        struct InfoPair
        {
            float timing;
            float bpm;
        };
        std::vector<InfoPair> bpm_list;
        unsigned int idx;
    };

    struct Difficulty
    {
        Instrument instrument;
        int difficulty;
    };

    struct LevelData
    {
        std::string title;
        std::string artist_name;
        std::string genre_name;
        float main_bpm;
        BpmInfo bpm_info;
        std::vector<Difficulty> difficulties;
    };

    struct BattleState
    {
        int max_hp{};
        int hp{};
        int score{};
        float clock_time{};
        Instrument instrument{};
        unsigned int difficulty{};
        BulletHellState bullet_hell_state{};
        RhythmState rhythm_state{};
        Acceptance acceptance{};
        ChartData chart_data;
        LevelData level_data;
    };
}