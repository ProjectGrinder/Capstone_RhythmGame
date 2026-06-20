#pragma once

namespace Game::World
{
    enum GraphicType
    {
        Bg1,
        Bg2,
        Bg3,
        Platform,
        PlatformTop,
        Effect,
        LevelNode,
        Npc1,
        Npc2,
        Other
    };
    static const char * graphic_type_string[] = {"Bg1","Bg2","Bg3","Platform","PlatformTop","Effect","LevelNode","Npc1","Npc2","Other"};
    struct GraphicData
    {
        GraphicType type;
        int src_rect[4];
        float dest_rect[4];
        float r,g,b,a;
        uint32_t layer;
        GraphicData(const GraphicType type) : type(type), src_rect{}, dest_rect{}, r(1), g(1), b(1), a(1), layer(static_cast<int>(type))
        {}
        explicit GraphicData(
                const GraphicType type,
                const int src0, const int src1, const int src2, const int src3,
                const uint32_t layer = 0,
                const float r = 1,
                const float g = 1,
                const float b = 1,
                const float a = 1) :
            type(type), src_rect{src0, src1, src2, src3}, r(r), g(g), b(b), a(a), layer(layer)
        {
            dest_rect[0] = static_cast<float>(src0 - src2);
            dest_rect[1] = static_cast<float>(src1 - src3);
            dest_rect[2] = static_cast<float>(src2 - src0);
            dest_rect[3] = static_cast<float>(src3 - src1);
        }
        explicit GraphicData(
                const GraphicType type, const int src0, const int src1, const int src2, const int src3,
                const float dest0, const float dest1, const float dest2, const float dest3,
                const uint32_t layer =0,
                const float r =1,
                const float g =1,
                const float b =1,
                const float a =1) :
            type(type), src_rect{src0, src1, src2, src3}, dest_rect{dest0, dest1, dest2, dest3},
            r(r), g(g), b(b), a(a), layer(layer)
        {}
    };

    struct SceneObject
    {
        float posX, posY;
        float size_x, size_y;
        uint16_t status_bit;
        uint16_t event_id = INVALID_ID;
        GraphicData graphic_data;
        Battle::ColliderData box_collider_data;
        Battle::ColliderData circle_collider_data;

        SceneObject() : posX(0), posY(0), size_x(1), size_y(1), status_bit(0), graphic_data(Other)
        {}
        SceneObject(const float x, const float y, const float s_x, const float s_y, const GraphicData &graphic_data)
            : posX(x), posY(y), size_x(s_x), size_y(s_y), status_bit(0), graphic_data(graphic_data) {}
        SceneObject(const float posX, const float posY, const float s_x, const float s_y, const uint16_t status_bit,
                const GraphicData &graphic_data, const Battle::ColliderData &box_c) :
            posX(posX), posY(posY), size_x(s_x), size_y(s_y), status_bit(status_bit), graphic_data(graphic_data), box_collider_data(box_c)
        {}
        SceneObject(
                const float posX,
                const float posY,
                const float s_x, const float s_y,
                const uint16_t status_bit,
                const uint16_t event_id,
                const GraphicData &graphic_data,
                const Battle::ColliderData &circle_c) :
            posX(posX),
            posY(posY),
            size_x(s_x), size_y(s_y),
            status_bit(status_bit),
            event_id(event_id),
            graphic_data(graphic_data),
            circle_collider_data(circle_c)
        {}
        SceneObject(
                float posX,
                float posY,
                const float s_x, const float s_y,
                uint16_t status_bit,
                uint16_t event_id,
                GraphicData graphic_data,
                Battle::ColliderData box_c,
                Battle::ColliderData circle_c) :
            posX(posX),
            posY(posY),
            size_x(s_x), size_y(s_y),
            status_bit(status_bit),
            event_id(event_id),
            graphic_data(graphic_data),
            box_collider_data(box_c),
            circle_collider_data(circle_c)
        {}
    };
    struct SceneRegistry
    {
        bool initialized = false;
        std::vector<SceneObject> scene_objects;

        SceneRegistry(const std::vector<SceneObject> &scene_objects) : scene_objects(scene_objects) {}

    };
} // namespace Game::Overview
