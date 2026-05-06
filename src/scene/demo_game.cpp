#include "scene.h"
#include "system.h"
#include "../../src/windows/windows_types.h"

extern "C" Window get_window_size();
const float half_height = static_cast<float>(get_window_size().height) / 2;
const float half_width = static_cast<float>(get_window_size().width) / 2;

using UI = Game::Battle::UIDisplay;

void init_graphics(const std::shared_ptr<Scene::DemoGame::TaskManager>& tm)
{
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    InputAttributeDescription sprite_vs_input_attributes[] = {
        InputAttributeDescription{"POSITION", InputType::R32G32B32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 12},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 20}};

    InputAttributeDescription sprite_ps_input_attributes[] = {
        InputAttributeDescription{"SV_POSITION", InputType::R32G32B32A32_FLOAT, 0},
        InputAttributeDescription{"TEXCOORD", InputType::R32G32_FLOAT, 16},
        InputAttributeDescription{"COLOR", InputType::R32G32B32A32_FLOAT, 24}};

    load_vertex_shader("shaders/vs/sprite.cso", "sprite_vs", sprite_vs_input_attributes, 3);

    load_pixel_shader("shaders/ps/sprite.cso", "sprite_ps", sprite_ps_input_attributes, 3);

    load_sprite("img/Default_Shot.dds", "bullet_sprite", 512, 512);
    load_sprite("img/test.dds", "test", 500, 500);

    load_sprite("img/Square.dds", "Square", 20, 20);
    load_sprite("img/Square20px.dds", "Square20px", 20, 20);

    load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
    load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
    load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
    load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
    load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
    load_sprite("img/rhythm/base_hold_disabled.dds", "hold_disabled", 100, 960);

    load_sprite("img/return.dds", "return", 1280, 720);
}

Game::Battle::BulletLoader Scene::DemoGame::create_bullet_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    const int left_padding = 0;
    const int left_padding2 = 15000;

    BulletLoader loader;
    loader.CreateBullet(left_padding, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 1000, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 1500, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 2000, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 2500, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 3000, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 3500, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 4000, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding + 4500, BulletData(rand_float(-500,500), rand_float(-300,300), 0, 0, 1000, 185));
    loader.CreateBullet(left_padding2, BulletData(rand_float(-500,500), -300, 0, rand_float(-135,-45), 1000, 176));
    loader.CreateBullet(left_padding2 + 1000, BulletData(-500, rand_float(-300,300), 0, rand_float(-45,45), 1000, 177));
    loader.CreateBullet(left_padding2 + 2000, BulletData(rand_float(-500,500), 300, 0, rand_float(45,135), 0, 1000, 178));
    loader.CreateBullet(left_padding2 + 3000, BulletData(500, rand_float(-300,300), 0, rand_float(-225,-135), 1000, 179));

    return (loader);
}

Game::Battle::ChartData Scene::DemoGame::create_note_test()
{
    Game::Battle::ChartData chart;

    for (int lane = 0; lane < 4; ++lane)
    {
        chart.lanes[lane].lane_number = lane;
        chart.lanes[lane].notes.clear();
        chart.lanes[lane].current_note = 0;
    }

    chart.lanes[0].notes.emplace_back(false, 7500, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[1].notes.emplace_back(false, 8000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[2].notes.emplace_back(false, 8500, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[3].notes.emplace_back(false, 9000, 0, Game::Battle::RhythmType::NORMAL);
    chart.lanes[0].notes.emplace_back(false, 9500, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[1].notes.emplace_back(false, 10000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[2].notes.emplace_back(false, 10500, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(false, 11000, 0, Game::Battle::RhythmType::ACCENT);
    chart.lanes[0].notes.emplace_back(true, 12000, 13000, Game::Battle::RhythmType::ACCENT);
    chart.lanes[3].notes.emplace_back(true, 12000, 13000, Game::Battle::RhythmType::ACCENT);

    return (chart);
}

Game::Render::Sprite Scene::assign_sprite(const int type)
{
    if (type == 1)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("accent")),
            .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 1};
    }
    if (type == 2)
    {
        return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("rain")),
            .pos = {{-75, 7.5, 0}, {75, 7.5, 0}, {75, -7.5, 0}, {-75, -7.5, 0}}, .layer = 2};
    }
    return Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")),
        .pos = {{-75, 15, 0}, {75, 15, 0}, {75, -15, 0}, {-75, -15, 0}}, .layer = 1};
}

void Scene::DemoGame::load_chart(
    std::shared_ptr<TaskManager> &tm,
    Game::Battle::ChartData &chart,
    Game::Rhythm::NoteField &field)
{
    using Timing = Game::Rhythm::Timing;
    using HoldStart = Game::Rhythm::HoldStart;
    using NoteType = Game::Rhythm::NoteType;
    using NoteStatus = Game::Rhythm::NoteStatus;

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
                        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
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
                        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
                        Game::Render::Transform{pos, 0, 0, 0});
                tm->create_entity<
                    Game::Rhythm::HoldConnect, NoteStatus,
                    Game::Render::Sprite,
                    Game::Render::Material,
                    Game::Render::Transform>(
                        Game::Rhythm::HoldConnect{lane.lane_number, note.timing, note.timing_end},
                        NoteStatus{0},
                        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("hold")), .pos = {{-25, 0, 0}, {25, 0, 0}, {25, 0, 0}, {-25, 0, 0}},
                        .layer = 0, .u0 = 0.0f, .v0 = 0.0f, .u1 = 1.0f, .v1 = 0.0f},
                        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
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
                        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
                        Game::Render::Transform{pos, 0, 0, 0});
            }

            ++lane.current_note;
        }
    }
    LOG_INFO("Finished loading chart")
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

Scene::DemoGame Scene::DemoGame::instance()
{
    static DemoGame instance;
    return (instance);
}

std::shared_ptr<Scene::DemoGame::TaskManager> Scene::DemoGame::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    init_graphics(tm);

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::RhythmState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Audio::SoundRegistry,
    Game::Rhythm::KeyInput, Game::BulletHell::Input>
    (
        Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(10),
        Game::Battle::RhythmState(1, 500, 60, 4.0f, 4.0f),
        init_bullet_graphic(),
        create_bullet_test(),
        create_pattern_container(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input());

    auto hit_sound = load_audio("audio/fishdam1", "player_hit");
    AudioCache *out = nullptr;
    load_audio_if_not_exist((AssetsRecord *) hit_sound, &out);

    tm->create_entity<Game::BulletHell::Player,
    Game::Render::Transform,
    Rotation,
    Velocity,
    Acceleration,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material, UI>(
        {}, Game::Render::Transform(0,-240), {}, {}, {},{},
        Game::Physics::CircularCollider(24),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("test")), .pos = {{-24, 24, 0}, {24, 24, 0}, {24, -24, 0}, {-24, -24, 0}}, .layer = 1},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
        UI(UI::BULLET)
    );

    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    tm->create_entity<Game::Rhythm::NoteField>(create_field());
    tm->create_entity<Game::Battle::LevelData>(Game::Battle::LevelData(
        "A World Without You",
        "Nakuya",
        "Digital Jpop",
        134.00f,
        create_bpm_info(),
        std::vector<Game::Battle::Difficulty>()));

    auto chart = create_note_test();
    auto field = create_field();

    load_chart(tm, chart, field);

    // temp judgement line
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, UI>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")), .pos = {{-500, 5, 0}, {500, 5, 0}, {500, -5, 0}, {-500, -5, 0}}},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, field.judge_level, 0}, 0, 0, 0},
        UI{UI::RHYTHM});

    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(5000, 2000, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(12000, 2000, Game::Battle::BULLET_HELL));

    const auto font = load_font("fonts/Klub04TT-Normal.dds", "Klub04TT-Normal", "fonts/Klub04TT-Normal.txt");

    // tm->create_entity<Game::Test::LifeText,
    // Game::Render::Text,
    // Game::Render::Material,
    // Game::Render::Transform>(
    //        Game::Test::LifeText{},
    //        Game::Render::Text{.font = font, .text = "0"},
    //        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
    //        Game::Render::Transform{Math::Point{-600, 200, 0}, 0, 0, 0}),
    //         UI{UI::BULLET};
    //
    // tm->create_entity<Game::Test::GrazeText,
    // Game::Render::Text,
    // Game::Render::Material,
    // Game::Render::Transform>(
    //        Game::Test::GrazeText{},
    //        Game::Render::Text{.font = font, .text = "0"},
    //        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
    //        Game::Render::Transform{Math::Point{400, 200, 0}, 0, 0, 0}),
    //         UI{UI::BULLET};

    tm->create_entity<Game::Battle::HpBarMax,
    Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>(
           Game::Battle::HpBarMax{},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square20px")), .pos = {{-55, 12.5, 0}, {55, 12.5, 0}, {55, -12.5, 0}, {-55, -12.5, 0}}, .layer = 6},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::HpBar,
    Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>(
           Game::Battle::HpBar{},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-50, 10, 0}, {50, 10, 0}, {50, -10, 0}, {-50, -10, 0}}, .color = {0.2f,1,0.2f}, .layer = 5},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, 300, 0}, 0, 0, 0});

    tm->create_entity<Game::Rhythm::JudgeText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform, UI>
    (
        Game::Rhythm::JudgeText(),
        Game::Render::Text{.font = font, .text = "", .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, half_height * 2 / 3, 0}, 0, 0, 0},
        UI{UI::RHYTHM});

    tm->create_entity<
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform, UI>
    (
        Game::Render::Text{.font = font, .text = "SCORE", .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{400, 400, 0}, 0, 0, 0},
        UI{UI::BOTH});
    tm->create_entity<Game::Battle::Score,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform, UI>
    (
        Game::Battle::Score(),
        Game::Render::Text{.font = font, .text = "0", .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{400, 350, 0}, 0, 0, 0},
        UI{UI::BOTH});

    // tm->create_entity<Game::Rhythm::Combo,
    // Game::Render::Text,
    // Game::Render::Material,
    // Game::Render::Transform, UI>
    // (
    //     Game::Rhythm::Combo(),
    //     Game::Render::Text{.font = font, .text = "", .layer = 5},
    //     Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
    //     Game::Render::Transform{Math::Point{0, half_height * 3 / 4, 0}, 0, 0, 0},
    //     UI{UI::RHYTHM});

    return (tm);
}

Scene::DemoGame::ResourceManager Scene::DemoGame::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoGame Scene.");
    return ResourceManager();
}