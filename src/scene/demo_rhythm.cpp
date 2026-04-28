#include "scene.h"
#include "system.h"
#include "utils/print_debug.h"
#include "../../src/windows/windows_types.h"

using Timing = Game::Rhythm::Timing;
using HoldStart = Game::Rhythm::HoldStart;
using NoteType = Game::Rhythm::NoteType;
using NoteStatus = Game::Rhythm::NoteStatus;

extern "C" Window get_window_size();

const float half_height = static_cast<float>(get_window_size().height) / 2;
const float half_width = static_cast<float>(get_window_size().width) / 2;

inline Game::Battle::ChartData create_demo_chart()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }
    /*
    // Pattern 1: Simple taps
    chart.lanes[0].notes.emplace_back(false, 5000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 6000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 7000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 8000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 9000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 9000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 11000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 11000, 0, Game::Battle::RhythmType::NORMAL);

    // Pattern 2: Accent notes
    chart.lanes[0].notes.emplace_back(false, 13000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 14000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 15000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 16000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 17000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 17000, 0, Game::Battle::RhythmType::ACCENT);

    // Pattern 3: Rain notes (only taps)
    for (int m = 0; m < 10; ++m)
    {
        chart.lanes[0].notes.emplace_back(false, 19000 + m * 100, 0, Game::Battle::RhythmType::RAIN);
    }
    for (int n = 0; n < 20; ++n)
    {
        chart.lanes[3].notes.emplace_back(false, 21000 + n * 50, 0, Game::Battle::RhythmType::RAIN);
    }

    // Pattern 4: Holds
    chart.lanes[0].notes.emplace_back(true, 24000, 25000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 26000, 27000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 28000, 30000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 29000, 30000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 31500, 33000, Game::Battle::RhythmType::ACCENT);
    */

    /*
    Pattern 5: Mixed notes
    chart.lanes[0].notes.emplace_back(false, 24000, 0);
    chart.lanes[0].notes.emplace_back(true, 25000, 26000);
    chart.lanes[1].notes.emplace_back(false, 26000, 0);
    chart.lanes[1].notes.emplace_back(true, 27000, 28000);
    chart.lanes[2].notes.emplace_back(true, 28000, 29000);
    chart.lanes[2].notes.emplace_back(false, 29500, 0);
    chart.lanes[3].notes.emplace_back(true, 30000, 31000);
    chart.lanes[3].notes.emplace_back(false, 31500, 0);
    chart.lanes[0].notes.emplace_back(false, 33000, 0);
    chart.lanes[2].notes.emplace_back(true, 33000, 34000);
    chart.lanes[0].notes.emplace_back(false, 35000, 0);
    chart.lanes[2].notes.emplace_back(true, 35000, 36000);
    */

    /*
    chart.lanes[0].notes.emplace_back(true, 5000, 6000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(true, 7000, 8000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(true, 9000, 12000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(true, 10000, 12000, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 13000, 15000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(true, 13000, 15000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(true, 13000, 15000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 13000, 15000, Game::Battle::RhythmType::ACCENT);
    */

    // vivid/stasis moment
    chart.lanes[0].notes.emplace_back(false, 5000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5120, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5240, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 5360, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5480, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5600, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 5720, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 5840, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 5960, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 6080, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6200, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 6320, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 6440, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 6560, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6680, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 6800, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 6920, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 7040, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(true, 7160, 11200, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 7640, 11200, Game::Battle::RhythmType::ACCENT);
    for (int m = 0; m < 16; ++m)
    {
        chart.lanes[1].notes.emplace_back(false, 8000 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(false, 8100 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
    }
    chart.lanes[1].notes.emplace_back(false, 11200, 0, Game::Battle::RhythmType::NORMAL);

    return (chart);
}

inline Game::Battle::BattleState create_battle_state()
{
    Game::Battle::BattleState state(100, 100, Game::Battle::Difficulty());
    state.current_phase = Game::Battle::RHYTHM;
    return (state);
}

inline Game::Battle::RhythmState create_rhythm_state()
{
    Game::Battle::RhythmState state(1, 100, 60, 6.0f, 6.0f);
    state.accept_loss.normal = 5;
    state.accept_loss.accent = 5;
    state.accept_loss.rain = 2;
    state.accept_loss.hold = 5;
    state.accept_loss.hold_end = 2;
    return (state);
}

inline Game::Rhythm::NoteField create_field()
{
    constexpr float note_width = 100.0f;
    // position based on window size
    const float spawn_level = half_height;
    const float judge_level = half_height * -2 / 3;
    constexpr float lane1_spawn = -1 * (note_width * 1.65f);
    constexpr float lane2_spawn = -1 * (note_width * 0.55f);
    constexpr float lane3_spawn = note_width * 0.55f;
    constexpr float lane4_spawn = note_width * 1.65f;
    constexpr float move_time = 5000.00f; // default speed 1
    return Game::Rhythm::NoteField(
        spawn_level,
        judge_level,
        lane1_spawn,
        lane2_spawn,
        lane3_spawn,
        lane4_spawn,
        move_time);
}

inline Game::Battle::BpmInfo create_bpm_info()
{
    Game::Battle::BpmInfo bpm;
    Game::Battle::BpmInfo::InfoPair info{};
    info.bpm = 134.00f;
    info.timing = 0;
    bpm.bpm_list.emplace_back(info);
    return (bpm);
}

inline Game::Battle::PhaseInfo create_phase_info()
{
    Game::Battle::PhaseInfo phase;
    Game::Battle::PhaseInfo::InfoPair info{};
    info.phase = Game::Battle::RHYTHM;
    info.timing = 0;
    phase.phase_list.emplace_back(info);
    return (phase);
}

auto sp1 = load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
auto sp2 = load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
auto sp0 = load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
auto sp_hold = load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
InputAttributeDescription sprite_vs_input_attributes[] = {
    InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12}};

InputAttributeDescription sprite_ps_input_attributes[] = {
    InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16}};
auto sprite_vs = load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 2);
auto sprite_ps = load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 2);
auto fn = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

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
                        assign_sprite(-1),
                        Game::Render::Material(sprite_vs, sprite_ps),
                        Game::Render::Transform{pos, 0, 0, 0});
                tm->create_entity<
                    Game::Rhythm::HoldConnect, NoteStatus,
                    Game::Render::Sprite,
                    Game::Render::Material,
                    Game::Render::Transform>(
                        Game::Rhythm::HoldConnect{lane.lane_number, note.timing, note.timing_end},
                        NoteStatus{0},
                        Game::Render::Sprite{.sp = sp_hold, .pos = {{-25, 0, 0}, {25, 0, 0}, {25, 0, 0}, {-25, 0, 0}},
                        .u0 = 0.0f, .v0 = 0.0f, .u1 = 1.0f, .v1 = 0.0f},
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

Scene::DemoRhythm Scene::DemoRhythm::instance()
{
    static DemoRhythm instance;
    return (instance);
}

std::shared_ptr<Scene::DemoRhythm::TaskManager> Scene::DemoRhythm::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1280, .scaleY = 720, .rotation = 0});

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::RhythmState,
    Game::Battle::ChartData,
    Game::Rhythm::KeyInput,
    Game::BulletHell::Input>
    (
        create_battle_state(),
        create_rhythm_state(),
        create_demo_chart(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input());

    tm->create_entity<Game::Battle::LevelData>(Game::Battle::LevelData(
        "A World Without You",
        "Nakuya",
        "Digital Jpop",
        134.00f,
        create_bpm_info(),
        std::vector<Game::Battle::Difficulty>()));

    // Create Lane
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    tm->create_entity<Game::Rhythm::NoteField>(create_field());

    tm->create_entity(
        Game::Render::Sprite{.sp = sp0, .pos = {{-500, 5, 0}, {500, 5, 0}, {500, -5, 0}, {-500, -5, 0}}},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{0, half_height * -2 / 3, 0}, 0, 0, 0});

    tm->create_entity(
        Game::Render::Text{.font = fn, .text = "PERFECT=0"},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{-600, 300, 0}, 0, 0, 0});
    tm->create_entity(
        Game::Render::Text{.font = fn, .text = "GREAT=0"},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{-600, 250, 0}, 0, 0, 0});
    tm->create_entity(
        Game::Render::Text{.font = fn, .text = "FINE=0"},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{-600, 200, 0}, 0, 0, 0});
    tm->create_entity(
        Game::Render::Text{.font = fn, .text = "MISS=0"},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{-600, 150, 0}, 0, 0, 0});

    auto chart = create_demo_chart();
    auto field = create_field();

    load_chart(tm, chart, field);

    tm->run_all();
    return (tm);
}

Scene::DemoRhythm::ResourceManager Scene::DemoRhythm::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return ResourceManager();
}
