#pragma once

#include "scene.h"
#include "system.h"

#include "game.h"
#include "game/utils/Bullethell_DSL/bullet_script.h"

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

    load_sprite("img/bullethell/Default_Shot.dds", "bullet_sprite", 512, 512);
    load_sprite("img/test.dds", "test", 500, 500);
    load_sprite("img/bullethell/BH_Player_Sprite.dds", "BH_Player_Sprite", 800, 1500);
    load_sprite("img/bullethell/Hitbox.dds", "Hitbox", 12, 12);

    load_sprite("img/Square.dds", "Square", 64, 64);
    load_sprite("img/Square64px.dds", "Square64px", 64, 64);
    load_sprite("img/ring16px.dds", "ring16px", 72, 72);

    load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
    load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
    load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
    load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
    load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
    load_sprite("img/rhythm/base_hold_disabled.dds", "hold_disabled", 100, 960);

    load_sprite("img/return.dds", "return", 1280, 720);

    load_sprite("img/level1_bg.dds", "level1_bg", 3840, 2160);
}

inline Game::Battle::LevelData create_level1_chartdata()
{
    Game::Battle::BpmInfo bpm;
    constexpr std::array timing_list = {17910, 66269, 123582};
    for (int m : timing_list)
    {
        Game::Battle::BpmInfo::InfoPair info{};
        info.bpm = 134.00f;
        info.timing = m;
        bpm.bpm_list.emplace_back(info);
    }
    return Game::Battle::LevelData(
    "A World Without You",
    "Nakuya",
    "Digital Jpop",
    134.00f,
    bpm,
    std::vector<Game::Battle::Difficulty>(), 142000
    );
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
                        .layer = 3, .u0 = 0.0f, .v0 = 0.0f, .u1 = 1.0f, .v1 = 0.0f},
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
    Game::BulletHell::BulletScript script{"dsl/ShotData.th0","dsl/Demo.th0"};

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::RhythmState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Render::AnimationDataRegistry,
    Game::Audio::SoundRegistry,
    Game::Rhythm::KeyInput, Game::BulletHell::Input>
    (
        Game::Battle::BattleState(100, 100, Game::Battle::Difficulty()),
        Game::Battle::BulletHellState(10),
        Game::Battle::RhythmState(1, 500, 247, 4.0f, 4.0f),
        std::move(script.bullet_registry),
        std::move(script.bullet_loader),
        std::move(script.pattern_container),
        init_anim_data(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input());

    auto hit_sound = load_audio("audio/fishdam1", "player_hit");
    AudioCache *out = nullptr;
    load_audio_if_not_exist((AssetsRecord *) hit_sound, &out);

    // Background
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("level1_bg")), .pos = {{-960, 540, 0}, {960, 540, 0}, {960, -540, 0}, {-960, -540, 0}},.layer = 0},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        {});

    // Border
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Battle::Border>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square64px")), .pos = {{-Game::HALF_WIDTH, Game::HALF_HEIGHT, 0}, {Game::HALF_WIDTH, Game::HALF_HEIGHT, 0}, {Game::HALF_WIDTH, -Game::HALF_HEIGHT, 0}, {-Game::HALF_WIDTH, -Game::HALF_HEIGHT, 0}},.layer = 4},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        {0,-Game::HALF_HEIGHT*1/3},{});

    tm->create_entity<Game::BulletHell::Player,
    Game::Render::Transform,
    Rotation,
    Velocity,
    AngularVelocity, Game::Physics::CircularCollider, Game::Render::Sprite, Game::Render::Material, Game::Render::Animator, Game::Render::Animation_Controller>(
        {}, Game::Render::Transform(0,-240), {}, {}, {},
        Game::Physics::CircularCollider(12),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("BH_Player_Sprite")),
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 1,
            .u0 = 0.f, .v0 = 0.f, .u1 = 200.f/800.f, .v1 = 250.f/1500.f},
        Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))},
        Game::Render::Animator{0}, Game::Render::Animation_Controller()
    );

    tm->create_entity<Game::BulletHell::PlayerHitbox, Game::Render::Transform, Game::Render::Sprite, Game::Render::Material>(
        Game::BulletHell::PlayerHitbox(7.5f), Game::Render::Transform(0,-240),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Hitbox")),
            .pos = {{-12, 12, 0}, {12, 12, 0}, {12, -12, 0}, {-12, -12, 0}}, .color = {1,1,1,0}, .layer = 10},
            Game::Render::Material{get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))}
    );

    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    tm->create_entity<Game::Rhythm::NoteField>(create_field());
    tm->create_entity<Game::Battle::LevelData>(create_level1_chartdata());

    auto chart = create_level1_chart();
    auto field = create_field();

    load_chart(tm, chart, field);

    // temp judgement line
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Rhythm::JudgementLine>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")), .pos = {{-450, 10, 0}, {450, 10, 0}, {450, -10, 0}, {-450, -10, 0}},.color = {1,1,1,0}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, field.judge_level, 0}, 0, 0, 0}, {}
        );

    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(16400, 1500, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(50149, 1500, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(65000, 1500, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(80000, 1500, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(122500, 1000, Game::Battle::RHYTHM));

    const auto font = load_font("fonts/Klub04TT-NoBG.dds", "Klub04TT-NoBG", "fonts/Klub04TT-Normal.txt");

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HPBarMax},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square64px")), .pos = {{-110, 12, 0}, {110, 12, 0}, {110, -12, 0}, {-110, -12, 0}}, .layer = 101},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 4/5, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HpBar},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-100, 10, 0}, {100, 10, 0}, {100, -10, 0}, {-100, -10, 0}}, .color = {0.2f,1,0.2f}, .layer = 100},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 4/5, 0}, 0, 0, 0});

    tm->create_entity<Game::Rhythm::JudgeText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::JudgeText(),
        Game::Render::Text{.font = font, .text = "", .layer = 100},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 2 / 3, 0}, 0, 0, 0}
        );

    tm->create_entity<
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Text{.font = font, .text = "SCORE", .color = Math::Color{0, 0, 0, 1}, .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{500, Game::HALF_HEIGHT * 4/5, 0}, 0, 0, 0});
    tm->create_entity<Game::Battle::Score,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::Score(),
        Game::Render::Text{.font = font, .text = "0", .color = Math::Color{0, 0, 0, 1}, .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{500, Game::HALF_HEIGHT * 4/5 - 50, 0}, 0, 0, 0}
        );

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent{Game::Battle::PhaseChangeText},
        Game::Render::Text{.font = font, .text = "", .layer = 105},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{0, Game::HALF_HEIGHT * 4/5, 0, 0, 0, 2.5f,2.5f,1});

    return (tm);
}

Scene::DemoGame::ResourceManager Scene::DemoGame::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting DemoGame Scene.");
    return ResourceManager();
}