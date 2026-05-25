#pragma once
#include <variant>

namespace Game::World
{
    struct SceneChangeEvent
    {
        uint16_t scene_id;
        SceneChangeEvent() = default;
        explicit SceneChangeEvent(const uint16_t& scene_id) : scene_id(scene_id) {}
    };

    struct DialogueEvent
    {
        System::ECS::pid dialogue_box_id = INVALID_PID;
        uint16_t dialogue_id;
        DialogueEvent() = default;
        explicit DialogueEvent(const uint16_t &dialogue_id) : dialogue_id(dialogue_id) {}
    };

    struct CutSceneEvent{};

    struct LevelNodeEvent
    {
        System::ECS::pid level_node_id =  INVALID_PID;
        uint16_t id;
        LevelNodeEvent() = default;
        explicit LevelNodeEvent(const uint16_t id) : id(id) {}
    };

    struct LockInputEvent
    {
        uint16_t lockBit = 0;
        LockInputEvent() = default;
        explicit LockInputEvent(const uint16_t lockBit) : lockBit(lockBit) {}
        explicit LockInputEvent(std::initializer_list<InputType> inputs)
        {
            for (auto input : inputs)
            {
                lockBit |= (1 << static_cast<uint16_t>(input));
            }
        }
    };
    struct UnlockInputEvent
    {
        uint16_t lockBit = 0;
        UnlockInputEvent()
        {
            lockBit = 7;
        }
        explicit UnlockInputEvent(const uint16_t lockBit) : lockBit(lockBit) {}
        explicit UnlockInputEvent(std::initializer_list<InputType> inputs)
        {
            for (auto input : inputs)
            {
                lockBit |= (1 << static_cast<uint16_t>(input));
            }
        }
    }; // Should unlock autonomously if event ends

    struct PanCameraEvent
    {
        System::ECS::pid dialogue_box_id;
        float duration;
        int order;
        PanCameraEvent() = default;
        explicit PanCameraEvent(const System::ECS::pid id, const float _duration, const int _order = 0)
        : dialogue_box_id(id), duration(_duration), order(_order) {}
    };

    struct ChangeNextEvent
    {
        uint16_t event_id;
        ChangeNextEvent() = default;
        explicit ChangeNextEvent(const uint16_t event_id) : event_id(event_id) {}
    };

    using Event = std::variant<
            SceneChangeEvent,
            DialogueEvent,
            CutSceneEvent,
            LevelNodeEvent,
            LockInputEvent,
            UnlockInputEvent,
            PanCameraEvent,
            ChangeNextEvent>;

    struct EventSequence
    {
        std::vector<Event> events;

        EventSequence()
        {}
        EventSequence(std::initializer_list<Event> events) : events(events){}
    };

    struct EventRegister
    {
        std::vector<EventSequence> event_sequences;

        EventRegister()
        {}
        EventRegister(std::initializer_list<EventSequence> event_sequences) : event_sequences(event_sequences){}
    };

} // namespace Game::Overview
