#pragma once
#include <utility>
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
        int timing;
        int timing_end;
        NoteData() : is_hold(false), timing(0), timing_end(0)
        {}
        explicit NoteData(const bool is_hold, const int timing, const int timing_end) : is_hold(is_hold), timing(timing), timing_end(timing_end)
        {}
    };

    struct LaneInfo
    {
        std::vector<NoteData> notes;
        int lane_number;
        unsigned int current_note;
        LaneInfo() : lane_number(0), current_note(0)
        {}
        explicit LaneInfo(const int lane_number) : lane_number(lane_number), current_note(0)
        {}
    };

    struct Difficulty
    {
        Instrument instrument;
        int difficulty;
        Difficulty() : instrument(NO_INSTRUMENT), difficulty(0)
        {}
        explicit Difficulty(const Instrument instrument,const int difficulty) : instrument(instrument), difficulty(difficulty)
        {}
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
        BpmInfo() : idx(0)
        {}
        explicit BpmInfo(const unsigned int idx) : idx(idx)
        {}
    };

    // use these structures

    struct BattleState
    {
        int max_hp;
        int hp;
        int score;
        int clock_time;
        int total_accept;
        int current_accept;
        int max_accept_gauge;
        Difficulty difficulty;
        BattleState() :
            max_hp(0), hp(0), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(0)
        {}
        explicit BattleState(
                const int max_hp = 0, const int max_accept_gauge = 0, const Difficulty difficulty = Difficulty()) :
            max_hp(max_hp), hp(0), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(max_accept_gauge), difficulty(difficulty)
        {}
    };

    struct BulletHellState
    {
        int graze;
        int iframe_time;
        BulletHellState(): graze(0), iframe_time(0)
        {}
    };

    struct RhythmState
    {
        int heal_hp;
        int base_score;
        int total_notes;
        float note_speed;
        RhythmState() : heal_hp(0), base_score(0), total_notes(0), note_speed(0)
        {}
        RhythmState(const int heal_hp, const int base_score, const int total_notes, const float note_speed) :
            heal_hp(heal_hp), base_score(base_score), total_notes(total_notes), note_speed(note_speed)
        {}
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
        LevelData() : main_bpm(0)
        {}
        explicit LevelData(
                std::string title,
                std::string artist_name,
                std::string genre_name,
                const float main_bpm,
                BpmInfo bpm_info,
                std::vector<Difficulty> difficulties) :
            title(std::move(title)), artist_name(std::move(artist_name)), genre_name(std::move(genre_name)), main_bpm(main_bpm),
            bpm_info(std::move(bpm_info)), difficulties(std::move(difficulties))
        {}
    };
}