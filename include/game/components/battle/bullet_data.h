#pragma once
#include <vector>

#include "game/components/physics/base_collider.h"
#include "game/components/render/sprite.h"

namespace Game::Battle
{
    // TODO : Put it somewhere else I think.
    constexpr size_t MAX_BULLETS_GRAPHIC = 128;
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

    struct BulletRegistry
    {
        std::vector<BulletGraphicMap> bulletGraphicMaps;
        explicit BulletRegistry(std::vector<BulletGraphicMap>& bulletGraphicMaps) : bulletGraphicMaps(std::move(bulletGraphicMaps))
        {}
    };

    struct BulletData
    {
        float posX, posY;
        float vel, rot;
        float acc, wvel;
        uint16_t patternID;
        int delay_frame;
        int graphicID;
        BulletData(
                const float posX,
                const float posY,
                const int delay_frame,
                const int graphicID) :
            posX(posX),
            posY(posY),
            vel(0),
            rot(0),
            acc(0),
            wvel(0),
            patternID(0),
            delay_frame(delay_frame),
            graphicID(graphicID)
        {}
        BulletData(
                const float posX,
                const float posY,
                const float vel,
                const float rot,
                const int delay_frame,
                const int graphicID) :
            posX(posX),
            posY(posY),
            vel(vel),
            rot(rot),
            acc(0),
            wvel(0),
            patternID(0),
            delay_frame(delay_frame),
            graphicID(graphicID)
        {}
        BulletData(
                const float posX,
                const float posY,
                const float vel,
                const float rot,
                const uint16_t patternID,
                const int delay_frame,
                const int graphicID) :
            posX(posX), posY(posY), vel(vel), rot(rot), acc(0), wvel(0), patternID(patternID), delay_frame(delay_frame), graphicID(graphicID)
        {}
        BulletData(
                const float posX,
                const float posY,
                const float vel,
                const float rot,
                const float acc,
                const float wvel,
                const int delay_frame,
                const int graphicID) :
            posX(posX),
            posY(posY),
            vel(vel),
            rot(rot),
            acc(acc),
            wvel(wvel),
            patternID(0),
            delay_frame(delay_frame),
            graphicID(graphicID)
        {}

        BulletData(
                const float posX,
                const float posY,
                const float vel,
                const float rot,
                const float acc,
                const float wvel,
                const uint16_t patternID,
                const int delay_frame,
                const int graphicID) :
            posX(posX), posY(posY), vel(vel), rot(rot), acc(acc), wvel(wvel), patternID(patternID), delay_frame(delay_frame), graphicID(graphicID)
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
}