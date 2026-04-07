#pragma once
#include <string>
#include <utility>
#include <vector>
#include <array>

namespace Game::Battle
{

    // intermediate structures
    enum Instrument
    {
        NO_INSTRUMENT,
    };

    enum CurrentPhase
    {
        NO_PHASE,
        BULLET_HELL,
        RHYTHM
    };

    enum RhythmType
    {
        NORMAL,
        ACCENT,
        RAIN,
    };

    struct NoteData
    {
        bool is_hold;
        int timing;
        int timing_end;
        RhythmType note_type;
        NoteData() : is_hold(false), timing(0), timing_end(0), note_type(NORMAL)
        {}
        explicit NoteData(const bool is_hold, const int timing, const int timing_end, const RhythmType note_type) : is_hold(is_hold), timing(timing), timing_end(timing_end), note_type(note_type)
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

    struct JudgementCount
    {
        int perfect_count;
        int great_count;
        int fine_count;
        int miss_count;
        JudgementCount() : perfect_count(0), great_count(0), fine_count(0), miss_count(0)
        {}
        explicit JudgementCount(const int perfect_count, const int great_count, const int fine_count, const int miss_count) : perfect_count(perfect_count), great_count(great_count), fine_count(fine_count), miss_count(miss_count)
        {}
    };

    struct BpmInfo
    {
        struct InfoPair
        {
            int timing;
            float bpm;
        };
        std::vector<InfoPair> bpm_list;
        unsigned int idx;
        BpmInfo() : idx(0)
        {}
        explicit BpmInfo(const unsigned int idx) : idx(idx)
        {}
    };

    struct PhaseInfo
    {
        struct InfoPair
        {
            int timing;
            CurrentPhase phase;
        };
        std::vector<InfoPair> phase_list;
        unsigned int idx;
        PhaseInfo() : idx(0)
        {}
        explicit PhaseInfo(const unsigned int idx) : idx(idx)
        {}
    };

    struct AcceptLoss
    {
        int normal;
        int accent;
        int rain;
        int hold;
        int hold_end;
        AcceptLoss() : normal(0), accent(0), rain(0), hold(0), hold_end(0)
        {}
        explicit AcceptLoss(const int normal, const int accent, const int rain, const int hold, const int hold_end) :
            normal(normal), accent(accent), rain(rain), hold(hold), hold_end(hold_end)
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
        JudgementCount judgement_count;
        CurrentPhase current_phase;
        BattleState() :
            max_hp(0), hp(0), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(0), current_phase(RHYTHM)
        {}
        explicit BattleState(
                const int max_hp, const int max_accept_gauge, const Difficulty difficulty) :
            max_hp(max_hp), hp(max_hp), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(max_accept_gauge), difficulty(difficulty), current_phase(BULLET_HELL)
        {}
    };

    struct BulletHellState
    {
        int graze;
        int iframe_time;
        int damage;
        BulletHellState(): graze(0), iframe_time(3000), damage(1)
        {}
    };

    struct RhythmState
    {
        int heal_hp;
        int base_score;
        int total_notes;
        float base_speed;
        float current_speed;
        bool speed_change;
        AcceptLoss accept_loss;
        RhythmState() : heal_hp(0), base_score(0), total_notes(0), base_speed(1.0f), current_speed(1.0f), speed_change(false)
        {}
        RhythmState(const int heal_hp, const int base_score, const int total_notes, const float base_speed, const float current_speed) :
            heal_hp(heal_hp), base_score(base_score), total_notes(total_notes), base_speed(base_speed), current_speed(current_speed), speed_change(false)
        {}
    };

    struct ChartData
    {
        std::array<LaneInfo, 4> lanes;
    };

    struct LevelData
    {
        std::string title;
        std::string artist_name;
        std::string genre_name;
        float main_bpm;
        BpmInfo bpm_info;
        PhaseInfo phase_info;
        std::vector<Difficulty> difficulties;
        LevelData() : main_bpm(0)
        {}
        explicit LevelData(
                std::string title,
                std::string artist_name,
                std::string genre_name,
                const float main_bpm,
                BpmInfo bpm_info,
                PhaseInfo phase_info,
                std::vector<Difficulty> difficulties) :
            title(std::move(title)), artist_name(std::move(artist_name)), genre_name(std::move(genre_name)), main_bpm(main_bpm),
            bpm_info(std::move(bpm_info)), phase_info(std::move(phase_info)), difficulties(std::move(difficulties))
        {}
    };
}