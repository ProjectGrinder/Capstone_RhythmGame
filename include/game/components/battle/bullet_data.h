#pragma once
#include <vector>

#include "../audio/audio.h"
#include "game/components/bullethell/pattern.h"
#include "game/components/physics/base_collider.h"
#include "maths/point.h"

namespace Game::Battle
{
    constexpr size_t MAX_BULLETS_GRAPHIC = 128;
    enum BulletType
    {
        Bullet, Booming, Laser
    };
    // enum class BulletAttribute {
    //     None       = 0,
    //     Homing     = 1 << 0,
    //     Bounce     = 1 << 1,
    //     Spin       = 1 << 2
    // };
    // inline BulletAttribute operator|(BulletAttribute a, BulletAttribute b) {
    //     return static_cast<BulletAttribute>(
    //         static_cast<int>(a) | static_cast<int>(b)
    //     );
    // }
    //
    // inline BulletAttribute operator&(BulletAttribute a, BulletAttribute b) {
    //     return static_cast<BulletAttribute>(
    //         static_cast<int>(a) & static_cast<int>(b)
    //     );
    // }
    //
    // inline bool hasAttribute(const BulletAttribute flags, const BulletAttribute flag) {
    //     return (flags & flag) != BulletAttribute::None;
    // }

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
        int src_rect[4];
        float dest_rect[4];
        float r,g,b,a;
        int bullet_spawn_sound;
        GraphicData() : src_rect{}, dest_rect{}, r(1), g(1), b(1), a(1), bullet_spawn_sound(1)
        {}
        explicit GraphicData(
                const int src0, const int src1, const int src2, const int src3,
                const float r = 1,
                const float g = 1,
                const float b = 1,
                const float a = 1,
                const int bullet_spawn_sound = 1) :
            src_rect{src0,src1,src2,src3}, r(r), g(g), b(b), a(a), bullet_spawn_sound(bullet_spawn_sound)
        {
            dest_rect[0] = static_cast<float>(src0 - src2);
            dest_rect[1] = static_cast<float>(src1 - src3);
            dest_rect[2] = static_cast<float>(src2 - src0);
            dest_rect[3] = static_cast<float>(src3 - src1);
        }
        explicit GraphicData(
                const int src0, const int src1, const int src2, const int src3,
                const float dest0, const float dest1, const float dest2, const float dest3,
                const float r,
                const float g,
                const float b,
                const float a,
                const int bullet_spawn_sound = 1) :
            src_rect{src0,src1,src2,src3}, dest_rect{dest0, dest1, dest2, dest3}, r(r), g(g), b(b), a(a), bullet_spawn_sound(bullet_spawn_sound)
        {}
    };

    struct BulletMovementData
    {
        float posX, posY;
        float vel, rot;
        float acc, wvel;

        BulletMovementData(const float posX = 0,
            const float posY = 0,
            const float vel = 0,
            const float rot = 0,
            const float acc = 0,
            const float wvel = 0):
        posX(posX), posY(posY), vel(vel), rot(rot), acc(acc), wvel(wvel){}
    };

    struct BulletTimingData
    {
        int delay_frame;
        int lifetime;

        BulletTimingData(const int delay_frame = 0, const int lifetime = 5000): delay_frame(delay_frame), lifetime(lifetime){}
    };

    struct BulletGraphicMap
    {
        ColliderData collider_data;
        GraphicData graphic_data;

        BulletGraphicMap()
        {}

        explicit BulletGraphicMap(
                const ColliderData &collider_data,
                const GraphicData &graphic_data = {}) :
            collider_data(collider_data),
            graphic_data(graphic_data)
        {}
    };

    struct StageBulletData
    {
        int graphicID;
        SpecialBulletData special_bullet_data;
        BulletTimingData bullet_timing_data;
        Math::Point size;
        float damage_mul;
        int pierce;

        explicit StageBulletData(
                const int graphicID = 0,
                const SpecialBulletData &special_bullet_data = {},
                const BulletTimingData &bullet_timing_data = {},
                const Math::Point size = Math::Point(1, 1),
                const float damage_mul = 1,
                const int pierce = 1) :
            special_bullet_data(special_bullet_data),
            graphicID(graphicID),
            bullet_timing_data(bullet_timing_data),
            size(size),
            damage_mul(damage_mul),
            pierce(pierce)
        {}
    };

    struct BulletRegistry
    {
        std::vector<BulletGraphicMap> bulletGraphicMaps;
        std::vector<StageBulletData> bulletStageMaps;
        BulletRegistry() {};
        explicit BulletRegistry(std::vector<BulletGraphicMap>& bulletGraphicMaps) : bulletGraphicMaps(std::move(bulletGraphicMaps))
        {}
        explicit BulletRegistry(std::vector<BulletGraphicMap>& bulletGraphicMaps, std::vector<StageBulletData>& bulletStageMaps) :
            bulletGraphicMaps(std::move(bulletGraphicMaps)), bulletStageMaps(std::move(bulletStageMaps))
        {}
    };

    struct BulletData
    {
        BulletMovementData movement_data;
        BulletHell::Pattern pattern;
        int bullet_id;

        BulletData(
                const int bullet_id,
                const BulletMovementData &movement_data,
                const BulletHell::Pattern &pattern = {}
                ) :
            movement_data(movement_data),
            pattern(pattern),
            bullet_id(bullet_id)
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
        bool initialized = false;
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
}