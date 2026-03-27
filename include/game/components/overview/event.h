#pragma once

namespace Game::Overview
{
    struct SceneChangeEvent
    {
        std::string scene;
        SceneChangeEvent() = default;
        explicit SceneChangeEvent(const std::string& scene) : scene(scene) {}
    };

    struct DialogueEvent
    {
        System::ECS::pid dialogue_box_id = UNASSIGNED;
        std::string dialogue;
        DialogueEvent() = default;
        explicit DialogueEvent(const System::ECS::pid dialogue_box_id, const std::string &dialogue) : dialogue_box_id(dialogue_box_id), dialogue(dialogue) {}
    };

    struct CutSceneEvent{};

    struct LevelNodeEvent
    {
        System::ECS::pid level_node_id = UNASSIGNED;
        uint16_t id;
        LevelNodeEvent() = default;
        explicit LevelNodeEvent(const uint16_t id) : id(id) {}
    };

    struct LockInputEvent
    {
        uint16_t lockBit = 0;
        LockInputEvent() = default;
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
        float x,y;
        PanCameraEvent() = default;
        explicit PanCameraEvent(const float x, const float y) : x(x), y(y) {}
    };

    using Event = std::variant<
            SceneChangeEvent,
            DialogueEvent,
            CutSceneEvent,
            LevelNodeEvent,
            LockInputEvent,
            UnlockInputEvent,
            PanCameraEvent
        >;

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
