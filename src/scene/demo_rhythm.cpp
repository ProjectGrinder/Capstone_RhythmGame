#include "scene.h"
#include "system.h"
#include "utils/print_debug.h"

using Position = Game::Physics::Position;
using Timing = Game::Rhythm::Timing;
using HoldStart = Game::Rhythm::HoldStart;
using NoteType = Game::Rhythm::NoteType;
using NoteStatus = Game::Rhythm::NoteStatus;

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

inline Position field_to_position(const int lane, const Game::Rhythm::NoteField &field)
{
    if (lane == 0)
        return Position{field.lane1_spawn, field.spawn_level};
    if (lane == 1)
        return Position{field.lane2_spawn, field.spawn_level};
    if (lane == 2)
        return Position{field.lane3_spawn, field.spawn_level};
    if (lane == 3)
        return Position{field.lane4_spawn, field.spawn_level};
    return Position{0, 0};
}

void Scene::DemoRhythm::load_chart(
    std::shared_ptr<TaskManager> &tm,
    Game::Battle::ChartData &chart,
    Game::Rhythm::NoteField &field)
{
    auto &[lanes] = chart;

    // repeat for each lane
    LOG_INFO("Loading chart...");
    for (auto &lane: lanes)
    {
        while (lane.current_note < lane.notes.size())
        {
            auto &note = lane.notes.at(lane.current_note);

            if (note.is_hold)
            {
                tm->create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                        field_to_position(lane.lane_number, field),
                        Timing{lane.lane_number, note.timing},
                        HoldStart{true},
                        NoteType{note.note_type},
                        NoteStatus{-1});
                tm->create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                        field_to_position(lane.lane_number, field),
                        Timing{lane.lane_number, note.timing_end},
                        HoldStart{false},
                        NoteType{-1},
                        NoteStatus{-1});
            }
            else
            {
                tm->create_entity<Position, Timing, HoldStart, NoteType, NoteStatus>(
                        field_to_position(lane.lane_number, field),
                        Timing{lane.lane_number, note.timing},
                        HoldStart{false},
                        NoteType{note.note_type},
                        NoteStatus{-1});
            }

            ++lane.current_note;
        }
    }
    LOG_INFO("Finishing loading chart")
}

std::vector<Scene::DemoRhythm::ComponentTuple> Scene::DemoRhythm::exit()
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return {};
}
