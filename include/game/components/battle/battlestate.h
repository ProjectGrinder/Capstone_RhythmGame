#pragma once
#include <string>
#include <utility>
#include <vector>

#include "game/components/bullethell/pattern.h"
#include "game/components/physics/base_collider.h"
#include "game/components/render/sprite.h"


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
            max_hp(0), hp(0), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(0), current_phase(BULLET_HELL)
        {}
        explicit BattleState(
                const int max_hp, const int max_accept_gauge, const Difficulty difficulty) :
            max_hp(max_hp), hp(0), score(0), clock_time(0), total_accept(0), current_accept(0), max_accept_gauge(max_accept_gauge), difficulty(difficulty), current_phase(BULLET_HELL)
        {}
    };

    struct BulletHellState
    {
        int graze;
        int iframe_time;
        int damage;
        BulletHellState(): graze(0), iframe_time(0), damage(0)
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

    // TODO : Put it somewhere else I think.
    enum BulletType
    {
        Bullet, Booming, Laser
    };
    struct SpecialBulletData
    {
        BulletType type;
        float size;
        int frame;
        SpecialBulletData() : type(Bullet), size(0), frame(0){}
        explicit SpecialBulletData(const BulletType type, const float size, const int frame) : type(type), size(size), frame(frame){}
    };
    struct ColliderData
    {
        Physics::ColliderType type;
        float offsetX, offsetY, colX, colY;
        ColliderData(): type(Physics::CIRCLE), offsetX(0), offsetY(0), colX(1), colY(1)
        {}
        ColliderData(const Physics::ColliderType type,
                const float col):
        type(type), offsetX(0), offsetY(0), colX(col), colY(col)
        {}
        ColliderData(const Physics::ColliderType type,
                const float colX,
                const float colY):
        type(type), offsetX(0), offsetY(0), colX(colX), colY(colY)
        {}
        ColliderData(const Physics::ColliderType type, const float offsetX, const float offsetY,
                const float colX,
                const float colY):
        type(type), offsetX(offsetX), offsetY(offsetY), colX(colX), colY(colY)
        {}
    };
    struct GraphicData
    {
        Render::Sprite sprite;
        float r,g,b,a;
        GraphicData() : sprite({}), r(1), g(1), b(1), a(1) {}
        explicit GraphicData(const Render::Sprite &sprite, const float r = 1, const float g = 1, const float b = 1, const float a = 1) :
            sprite(sprite), r(r), g(g), b(b), a(a) {}
    };

    struct BulletGraphicMap
    {
        ColliderData collider_data;
        GraphicData graphic_data;
        SpecialBulletData special_bullet_data;
        float damage_mul;
        int pierce;
        int lifetime;

        BulletGraphicMap() : damage_mul(0), pierce(1), lifetime(10)
        {}

        explicit BulletGraphicMap(
                const ColliderData &collider_data,
                const GraphicData &graphic_data = {},
                const SpecialBulletData &special_bullet_data = {},
                const float damage_mul = 1,
                const int pierce = 1,
                const int lifetime = 10 ) :
            collider_data(collider_data), graphic_data(graphic_data), special_bullet_data(special_bullet_data), damage_mul(damage_mul), pierce(pierce), lifetime(lifetime)
        {}
    };

    inline std::array<BulletGraphicMap, 128> bulletGraphicMap;

    struct BulletData
    {
        float posX, posY;
        BulletHell::Patterns patterns;
        int delay_frame;
        int graphicID;
        BulletData(const float posX, const float posY, BulletHell::Patterns patterns, const int delay_frame, const int graphicID):
            posX(posX), posY(posY), patterns(std::move(patterns)), delay_frame(delay_frame), graphicID(graphicID)
        {}
    };

    struct BulletBatch
    {
        int frame;
        std::vector<BulletData> bullets;
    };

    struct BulletLoader
    {
        // this should be global.
        int current_frame;
        int pointer;

        std::vector<BulletBatch> batches;

        BulletLoader() :
            current_frame(0), pointer(0)
        {}

        BulletData CreateBullet(const int frame, const BulletData& data)
        {
            if (batches.empty() || batches.back().frame != frame)
            {
                batches.push_back({ frame, {} });
            }

            batches.back().bullets.push_back(data);
            return batches.back().bullets.back();
        }
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