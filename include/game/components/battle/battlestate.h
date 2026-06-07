#pragma once
#include <array>
#include <string>
#include <utility>
#include <vector>

namespace Game::Battle
{

    // intermediate structures
    enum DifficultyType
    {
        LIGHT, // easy
        SPARK, // normal
        BLAZE, // hard
        ASTRA // extra (optional)
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
        explicit NoteData(const bool is_hold, const int timing, const int timing_end, const RhythmType note_type) :
            is_hold(is_hold), timing(timing), timing_end(timing_end), note_type(note_type)
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
        DifficultyType difficulty;
        int level;
        Difficulty() : difficulty(LIGHT), level(1)
        {}
        Difficulty(const DifficultyType difficulty, const int level) :
            difficulty(difficulty), level(level)
        {}
        // max level 10
    };

    struct JudgementCount
    {
        int perfect_count;
        int great_count;
        int fine_count;
        int miss_count;
        JudgementCount() : perfect_count(0), great_count(0), fine_count(0), miss_count(0)
        {}
        explicit JudgementCount(
                const int perfect_count, const int great_count, const int fine_count, const int miss_count) :
            perfect_count(perfect_count), great_count(great_count), fine_count(fine_count), miss_count(miss_count)
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
        unsigned int idx = 0;
        BpmInfo() = default;
        BpmInfo(const std::vector<InfoPair> &bpm_list) : bpm_list(bpm_list)
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

    enum PlayerState
    {
        PLAY,
        DEAD,
        FINISH
    };

    // use these structures

    struct BattleState
    {
        int max_hp;
        int hp;
        int clock_time; // initialize clock with 3-second wait period
        int current_accept;
        int max_accept_gauge;
        PlayerState player_state;
        Difficulty difficulty;
        JudgementCount judgement_count;
        CurrentPhase current_phase;
        BattleState() :
            max_hp(0),
            hp(0),
            clock_time(-3000000),
            current_accept(0),
            max_accept_gauge(0),
            player_state(PLAY),
            current_phase(BULLET_HELL)
        {}
        explicit BattleState(const int max_hp, const int max_accept_gauge, const Difficulty difficulty) :
            max_hp(max_hp),
            hp(max_hp),
            clock_time(-3000000),
            current_accept(0),
            max_accept_gauge(max_accept_gauge),
            player_state(PLAY),
            difficulty(difficulty),
            current_phase(BULLET_HELL)
        {}
    };

    struct BulletHellState
    {
        int graze;
        int iframe_time;
        int damage;
        BulletHellState(const int damage = 1) : graze(0), iframe_time(0), damage(damage)
        {}
    };

    struct RhythmState
    {
        int heal_hp;
        int accept_gain;
        int total_notes;
        float base_speed;
        float current_speed;
        bool speed_change;
        float accuracy;
        float apn; // accuracy per note
        std::string accuracy_text;
        AcceptLoss accept_loss;
        RhythmState() :
            heal_hp(0), accept_gain(0), total_notes(0), base_speed(1.0f), current_speed(1.0f), speed_change(false), accuracy(0.00f), apn(0.00f)
        {}
        RhythmState(
                const int heal_hp,
                const int accept_gain,
                const int total_notes,
                const float base_speed,
                const float current_speed) :
            heal_hp(heal_hp),
            accept_gain(accept_gain),
            total_notes(total_notes),
            base_speed(base_speed),
            current_speed(current_speed),
            speed_change(false),
            accuracy(0.00f),
            apn(0.00f)
        {}
    };

    struct ChartData
    {
        std::array<LaneInfo, 4> lanes;
    };

    // TODO : Should include max_acceptance_gauge
    struct LevelData
    {
        std::string title;
        std::string artist_name;
        std::string genre_name;
        float main_bpm;
        BpmInfo bpm_info;
        Difficulty difficulty;
        int duration;
        explicit LevelData(
                std::string title,
                std::string artist_name,
                const float main_bpm,
                BpmInfo bpm_info,
                const Difficulty difficulty,
                const int duration) :
            title(std::move(title)),
            artist_name(std::move(artist_name)),
            main_bpm(main_bpm),
            bpm_info(std::move(bpm_info)),
            difficulty(difficulty),
            duration(duration)
        {}
    };
} // namespace Game::Battle
