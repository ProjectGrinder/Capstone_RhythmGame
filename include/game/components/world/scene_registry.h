#pragma once

namespace Game::Overview
{
    struct SceneObject
    {
        float posX, posY;
        uint16_t status_bit;
        uint16_t event_id = INVALID_ID;
        Battle::GraphicData graphic_data;
        Battle::ColliderData box_collider_data;
        Battle::ColliderData circle_collider_data;


        SceneObject() : posX(0), posY(0), status_bit(0)
        {}
        SceneObject(const float x, const float y) : posX(x), posY(y), status_bit(0) {}
        SceneObject(const float posX, const float posY, const uint16_t status_bit,
                const Battle::GraphicData &graphic_data, const Battle::ColliderData &box_c) :
            posX(posX), posY(posY), status_bit(status_bit), graphic_data(graphic_data), box_collider_data(box_c)
        {}
        SceneObject(
                const float posX,
                const float posY,
                const uint16_t status_bit,
                const uint16_t event_id,
                const Battle::GraphicData &graphic_data,
                const Battle::ColliderData &circle_c) :
            posX(posX),
            posY(posY),
            status_bit(status_bit),
            event_id(event_id),
            graphic_data(graphic_data),
            circle_collider_data(circle_c)
        {}
        SceneObject(
                float posX,
                float posY,
                uint16_t status_bit,
                uint16_t event_id,
                Battle::GraphicData graphic_data,
                Battle::ColliderData box_c,
                Battle::ColliderData circle_c) :
            posX(posX),
            posY(posY),
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
