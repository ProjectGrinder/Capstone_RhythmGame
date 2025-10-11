#pragma once
#include <vector>
#include <string>

namespace Game::Battle
{

    // intermediate structures
    enum Instrument
    {
        NO_INSTRUMENT,
    };

    struct NoteData
    {
        bool is_hold;
        float timing;
        float timing_end;
    };

    struct LaneInfo
    {
        std::vector<NoteData> notes;
        int lane_number;
        unsigned int current_note;
    };

    struct Difficulty
    {
        Instrument instrument;
        int difficulty;
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

    // use these structures

    struct BattleState
    {
        int max_hp;
        int hp;
        int score;
        float clock_time;
        int total_accept;
        int current_accept;
        int max_accept_gauge;
        Difficulty difficulty;
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

    struct ChartData
    {
        LaneInfo lanes[4];
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
}