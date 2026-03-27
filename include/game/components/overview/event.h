#pragma once

namespace Game::Overview
{
    struct SceneChange
    {
        std::string scene;
        SceneChange() = default;
        explicit SceneChange(const std::string& scene) : scene(scene) {}
    };

    struct Dialogue
    {
        System::ECS::pid dialogue_box_id = UNASSIGNED;
        std::string dialogue;
        Dialogue() = default;
        explicit Dialogue(const System::ECS::pid dialogue_box_id, const std::string &dialogue) : dialogue_box_id(dialogue_box_id), dialogue(dialogue) {}
    };

    struct CutScene{};

    struct LevelNode
    {
        uint16_t id;
        LevelNode() = default;
        explicit LevelNode(const uint16_t id) : id(id) {}
    };

    struct LockInput
    {
        uint16_t lockBit = 0;
        LockInput() = default;
        explicit LockInput(std::initializer_list<InputType> inputs)
        {
            for (auto input : inputs)
            {
                lockBit |= (1 << static_cast<uint16_t>(input));
            }
        }
    };
    struct UnlockInput
    {
        uint16_t lockBit = 0;
        UnlockInput()
        {
            lockBit = 7;
        }
        explicit UnlockInput(std::initializer_list<InputType> inputs)
        {
            for (auto input : inputs)
            {
                lockBit |= (1 << static_cast<uint16_t>(input));
            }
        }
    }; // Should unlock autonomously if event ends

    struct PanCamera
    {
        float x,y;
        PanCamera() = default;
        explicit PanCamera(const float x, const float y) : x(x), y(y) {}
    };

    using Event = std::variant<
            SceneChange,
            Dialogue,
            CutScene,
            LevelNode,
            LockInput,
            UnlockInput,
            PanCamera
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
