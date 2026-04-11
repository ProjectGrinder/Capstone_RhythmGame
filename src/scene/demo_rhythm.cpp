#include "scene.h"
#include "system.h"
#include "utils/print_debug.h"

using Timing = Game::Rhythm::Timing;
using HoldStart = Game::Rhythm::HoldStart;
using NoteType = Game::Rhythm::NoteType;
using NoteStatus = Game::Rhythm::NoteStatus;

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

auto sp1 = load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
auto sp2 = load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
auto sp0 = load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
InputAttributeDescription sprite_vs_input_attributes[] = {
    InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12}};

InputAttributeDescription sprite_ps_input_attributes[] = {
    InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16}};
auto sprite_vs = load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 2);
auto sprite_ps = load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 2);

inline Math::Point field_to_point(const int lane, const Game::Rhythm::NoteField &field)
{
    if (lane == 0)
        return Math::Point{field.lane1_spawn, field.spawn_level, 0};
    if (lane == 1)
        return Math::Point{field.lane2_spawn, field.spawn_level, 0};
    if (lane == 2)
        return Math::Point{field.lane3_spawn, field.spawn_level, 0};
    if (lane == 3)
        return Math::Point{field.lane4_spawn, field.spawn_level, 0};
    return Math::Point{0, 0, 0};
}

inline Game::Render::Sprite assign_sprite(const int type)
{
    if (type == 1)
    {
        return Game::Render::Sprite{.sp = sp1,
            .pos = {{-50, 10, 0}, {50, 10, 0}, {50, -10, 0}, {-50, -10, 0}}};
    }
    if (type == 2)
    {
        return Game::Render::Sprite{.sp = sp2,
            .pos = {{-50, 5, 0}, {50, 5, 0}, {50, -5, 0}, {-50, -5, 0}}};
    }
    return Game::Render::Sprite{.sp = sp0,
            .pos = {{-50, 10, 0}, {50, 10, 0}, {50, -10, 0}, {-50, -10, 0}}};
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
            auto pos = field_to_point(lane.lane_number, field);

            if (note.is_hold)
            {
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing},
                        HoldStart{true},
                        NoteType{note.note_type},
                        NoteStatus{0},
                        assign_sprite(note.note_type),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing_end},
                        HoldStart{false},
                        NoteType{-1},
                        NoteStatus{0},
                        assign_sprite(note.note_type),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
            }
            else
            {
                tm->create_entity<
                Timing, HoldStart, NoteType, NoteStatus,
                Game::Render::Sprite,
                Game::Render::Material,
                Game::Render::Transform>(
                        Timing{lane.lane_number, note.timing},
                        HoldStart{false},
                        NoteType{note.note_type},
                        NoteStatus{0},
                        assign_sprite(note.note_type),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
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
