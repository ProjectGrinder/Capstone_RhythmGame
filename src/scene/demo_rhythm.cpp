#include "scene.h"
#include "utils/print_debug.h"

using Timing = Game::Rhythm::Timing;
using HoldStart = Game::Rhythm::HoldStart;
using NoteType = Game::Rhythm::NoteType;
using NoteStatus = Game::Rhythm::NoteStatus;

inline Game::Battle::ChartData create_demo_chart()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

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
        chart.lanes[0].notes.emplace_back(false, 8000 + m * 200, 0, Game::Battle::RhythmType::RAIN);
        chart.lanes[1].notes.emplace_back(false, 8000 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[2].notes.emplace_back(false, 8100 + m * 200, 0, Game::Battle::RhythmType::NORMAL);
        chart.lanes[3].notes.emplace_back(false, 8100 + m * 200, 0, Game::Battle::RhythmType::RAIN);
    }
    chart.lanes[1].notes.emplace_back(false, 11200, 0, Game::Battle::RhythmType::NORMAL);

    return (chart);
}

inline Game::Battle::BattleState create_battle_state()
{
    Game::Battle::BattleState state(100, Game::Battle::Difficulty());
    state.current_phase = Game::Battle::RHYTHM;
    return (state);
}

inline Game::Battle::RhythmState create_rhythm_state()
{
    Game::Battle::RhythmState state(1, 10, 60, 6.0f, 6.0f);
    state.accept_loss.normal = 5;
    state.accept_loss.accent = 5;
    state.accept_loss.rain = 2;
    state.accept_loss.hold = 5;
    state.accept_loss.hold_end = 2;
    state.apn = 100.00f / static_cast<float>(state.total_notes);
    return (state);
}

Game::Render::Text Scene::write_difficulty(const Game::Battle::Difficulty difficulty)
{
    // font must be loaded first!
    Game::Render::Text text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "", .color = Math::Color{0, 0, 0, 1}, .layer = 51};

    switch (difficulty.difficulty)
    {
    case Game::Battle::LIGHT:
        text.text = "LIGHT " + std::to_string(difficulty.level);
        text.color = Game::DIFF_COLOR[0];
        break;

    case Game::Battle::SPARK:
        text.text = "SPARK " + std::to_string(difficulty.level);
        text.color = Game::DIFF_COLOR[1];
        break;

    case Game::Battle::BLAZE:
        text.text = "BLAZE " + std::to_string(difficulty.level);
        text.color = Game::DIFF_COLOR[2];
        break;

    case Game::Battle::ASTRA:
        text.text = "ASTRA " + std::to_string(difficulty.level);
        text.color = Game::DIFF_COLOR[3];
        break;

    default:
        text.text = std::to_string(difficulty.level);
    }
    return (text);
}

auto sp_accent = load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
auto sp_rain = load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
auto sp_normal = load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
auto sp_disabled = load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
auto sp_hold = load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
auto sp_hold_disable = load_sprite("img/rhythm/base_hold_disabled.dds", "hold_disabled", 100, 960);
InputAttributeDescription sprite_vs_input_attributes[] = {
    InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
    InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

InputAttributeDescription sprite_ps_input_attributes[] = {
    InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
    InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
    InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};
auto sprite_vs = load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 3);
auto sprite_ps = load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 3);
auto fn = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

Math::Point Scene::lane_to_point(const int lane)
{
    switch (lane)
    {
    case 0:
        return Math::Point{Game::LANE1, Game::SPAWN_LEVEL, 0};
    case 1:
            return Math::Point{Game::LANE2, Game::SPAWN_LEVEL, 0};
    case 2:
            return Math::Point{Game::LANE3, Game::SPAWN_LEVEL, 0};
    case 3:
            return Math::Point{Game::LANE4, Game::SPAWN_LEVEL, 0};
    default:
        return Math::Point{0, 0, 0};
    }
}

Game::Render::Sprite Scene::assign_sprite(const int type)
{
    if (type == 1)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("accent")),
            .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 4};
    }
    if (type == 2)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("rain")),
            .pos = {{-75, 7.5, 0}, {75, 7.5, 0}, {75, -7.5, 0}, {-75, -7.5, 0}}, .layer = 5};
    }
    return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")),
        .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 4};
}

void Scene::DemoRhythm::load_chart(
    std::shared_ptr<TaskManager> &tm,
    Game::Battle::ChartData &chart)
{

    // repeat for each lane
    LOG_INFO("Loading chart...");
    for (auto &lane: chart.lanes)
    {
        while (lane.current_note < lane.notes.size())
        {
            auto &note = lane.notes.at(lane.current_note);
            auto pos = lane_to_point(lane.lane_number);

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
                        .layer = 0, .u0 = 0.0f, .v0 = 0.0f, .u1 = 1.0f, .v1 = 0.0f},
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
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::RhythmState,
    Game::Battle::ChartData,
    Game::Audio::SoundRegistry>
    (
        create_battle_state(),
        create_rhythm_state(),
        create_demo_chart(),
        Game::Audio::init_battle_sounds(0));

    // InputManager
    tm->create_entity<Game::Input>(Game::Input());

    // tm->create_entity<Game::Battle::LevelData>(create_level1_chartdata());

    // Create Lane
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    // tm->create_entity<Game::Rhythm::NoteField>(create_field());

    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = sp_normal, .pos = {{-500, 5, 0}, {500, 5, 0}, {500, -5, 0}, {-500, -5, 0}}},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * -2 / 3, 0}, 0, 0, 0});

    tm->create_entity<Game::Rhythm::JudgeText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::JudgeText(),
        Game::Render::Text{.font = fn, .text = "", .layer = 5},
        Game::Render::Material(sprite_vs, sprite_ps),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 2 / 3, 0}, 0, 0, 0});

    auto chart = create_demo_chart();

    load_chart(tm, chart);

    tm->run_all();
    return (tm);
}

Scene::DemoRhythm::ResourceManager Scene::DemoRhythm::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoRhythm Scene.");
    return ResourceManager();
}
