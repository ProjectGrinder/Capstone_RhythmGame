#pragma once

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
        int timing;
        int timing_end;
    };

    struct LaneInfo
    {
        int lane_number;
        std::vector<NoteData> notes;
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
        std::vector<std::pair<float, float>> bpm_list;
        std::vector<std::pair<Instrument, int>> difficulty;
    };

    struct BattleState
    {
        int max_hp;
        int hp;
        int score;
        float clock_time;
        Instrument instrument;
        unsigned int difficulty;
        BulletHellState bullet_hell_state;
        RhythmState rhythm_state;
        Acceptance acceptance;
        ChartData chart_data;
        LevelData level_data;
    };
}