#pragma once

#include "scene.h"
#include "game.h"
#include "game/utils/Bullethell_DSL/bullet_script.h"
#include "game/utils/rhythm_chart/level_02.h"

void init_graphics(const std::shared_ptr<Scene::Level2::TaskManager>& tm)
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
    load_sprite("img/gauge_bar.dds", "gauge", 960, 120);
    load_sprite("img/letter_rank.dds", "rank", 1280, 128);

    load_sprite("img/rhythm/base_accent.dds", "accent", 200, 40);
    load_sprite("img/rhythm/base_rain.dds", "rain", 200, 20);
    load_sprite("img/rhythm/base_normal.dds", "normal", 200, 40);
    load_sprite("img/rhythm/base_disabled.dds", "disabled", 200, 40);
    load_sprite("img/rhythm/base_hold.dds", "hold", 100, 960);
    load_sprite("img/rhythm/base_hold_disabled.dds", "hold_disabled", 100, 960);
    load_sprite("img/rhythm/note_border.dds", "note_border", 200, 40);
    load_sprite("img/rhythm/judge_text.dds", "judge", 1280, 640);

    load_sprite("img/return.dds", "return", 1280, 720);

    load_sprite("img/level1_bg.dds", "level1_bg", 3840, 2160);

    load_font("fonts/Klub04TT-NoBG.dds", "Klub04TT-NoBG", "fonts/Klub04TT-Normal.txt");
}

void init_battle_components(const std::shared_ptr<Scene::Level2::TaskManager>& tm)
{
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
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Battle::Backdrop>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-Game::HALF_WIDTH, Game::HALF_HEIGHT, 0}, {Game::HALF_WIDTH, Game::HALF_HEIGHT, 0}, {Game::HALF_WIDTH, -Game::HALF_HEIGHT, 0}, {-Game::HALF_WIDTH, -Game::HALF_HEIGHT, 0}},
            .color = {0, 0, 0, 0.4f}, .layer = 1},
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
            .pos = {{-32, 40, 0}, {32, 40, 0}, {32, -40, 0}, {-32, -40, 0}}, .layer = 2,
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

    // judgement line
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Rhythm::JudgementLine>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-350, 5, 0}, {350, 5, 0}, {350, -5, 0}, {-350, -5, 0}},.color = {1,1,1,0}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, Game::JUDGE_LEVEL, 0}, 0, 0, 0}, {}
        );

    // lane lines
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Rhythm::LaneLine>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-3, 0, 0}, {3, 0, 0}, {3, 0, 0}, {-3, 0, 0}},.color = {1,1,1,0.2f}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{(Game::LANE1+Game::LANE2)/2, -Game::HALF_HEIGHT, 0}, 0, 0, 0}, {});
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Rhythm::LaneLine>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-3, 0, 0}, {3, 0, 0}, {3, 0, 0}, {-3, 0, 0}},.color = {1,1,1,0.2f}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{(Game::LANE2+Game::LANE3)/2, -Game::HALF_HEIGHT, 0}, 0, 0, 0}, {});
    tm->create_entity<Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform, Game::Rhythm::LaneLine>
    (
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-3, 0, 0}, {3, 0, 0}, {3, 0, 0}, {-3, 0, 0}},.color = {1,1,1,0.2f}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{(Game::LANE3+Game::LANE4)/2, -Game::HALF_HEIGHT, 0}, 0, 0, 0}, {});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::AcceptBarMax},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("gauge")), .pos = {{-500, 20, 0}, {500, 20, 0}, {500, -20, 0}, {-500, -20, 0}}, .color = {0.8f, 0.8f, 0.8f}, .layer = 101},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 9/10, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::AcceptBarMax},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{144, 20, 0}, {148, 20, 0}, {148, -20, 0}, {144, -20, 0}}, .color = {0.9f, 0.9f, 0.9f}, .layer = 101},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 9/10, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::AcceptBar},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-365, 20, 0}, {365, 20, 0}, {365, -20, 0}, {-365, -20, 0}}, .color = {0, 0.5f, 1}, .layer = 100},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 9/10, 0}, 0, 0, 0});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HPBarMax},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("gauge")), .pos = {{-200, 25, 0}, {200, 25, 0}, {200, -25, 0}, {-200, -25, 0}}, .color = {1, 1, 1}, .layer = 101},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-Game::HALF_WIDTH * 4/5, 0, 0}, 0, 0, PI/2});

    tm->create_entity(
           Game::Battle::UIComponent{Game::Battle::HpBar},
           Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-150, 25, 0}, {150, 25, 0}, {150, -25, 0}, {-150, -25, 0}}, .color = {0, 1, 0}, .layer = 100},
           Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
           Game::Render::Transform{Math::Point{-Game::HALF_WIDTH * 4/5, 0, 0}, 0, 0, PI/2});

    tm->create_entity<Game::Rhythm::JudgeText,
    Game::Render::Sprite,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::JudgeText(),
        Game::Render::Sprite{
            .sp = get_assets_record_ptr(get_assets_id("judge")),
            .pos = {{-120, 30, 0}, {120, 30, 0}, {120, -30, 0}, {-120, -30, 0}},
            .layer = 100,
            .u0 = 0.5,
            .v0 = 0.5,
            .u1 = 1,
            .v1 = 0.75},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, Game::HALF_HEIGHT * 7/10, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent{Game::Battle::SongTitle},
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Strike Against The World!", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{-Game::HALF_WIDTH * 37/40 - 10, Game::HALF_HEIGHT * 4/5, 0}, 0, 0, 0});
    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent{Game::Battle::ArtistName},
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Pooh5821", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{-Game::HALF_WIDTH * 37/40 - 10, Game::HALF_HEIGHT * 11/15, 0, 0, 0, 0.8f, 0.8f, 0.8f});
    tm->create_entity<Game::Battle::UIComponent,
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{0, 0, 0}, {400, 0, 0}, {400, -200, 0}, {0, -200, 0}}, .color = {1,1,1, 0.7f}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{-Game::HALF_WIDTH * 19/20, Game::HALF_HEIGHT * 9/10, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-100, 30, 0}, {100, 30, 0}, {100, -30, 0}, {-150, -30, 0}}, .color = {1,1,1}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3, Game::HALF_HEIGHT * 1/10, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "ACCURACY", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3 - 75, Game::HALF_HEIGHT * 3/20 + 10, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::AccuracyText),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "0.00%", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3 - 75, Game::HALF_HEIGHT * 1/10 - 15, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
        Game::Render::Sprite,
        Game::Render::Material,
        Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-100, 30, 0}, {100, 30, 0}, {100, -30, 0}, {-150, -30, 0}}, .color = {1,1,1}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3, Game::HALF_HEIGHT * -1/5, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "GRAZE", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3 - 75, Game::HALF_HEIGHT * -3/20 + 10, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::GrazeText),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "0", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3 - 75, Game::HALF_HEIGHT * -1/5 - 15, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::StatBox),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "/ 20", .color = Math::Color{0, 0, 0, 1}, .layer = 51},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{Game::HALF_WIDTH * 2/3 - 35, Game::HALF_HEIGHT * -1/5 - 15, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::TransitionText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::TransitionText(),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "", .layer = 105},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{0, Game::HALF_HEIGHT * 4/5, 0, 0, 0, 2.5f,2.5f,1});

    // key for each lane
    // not sure how to keybind it
    tm->create_entity<Game::Rhythm::KeyText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::KeyText(),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "S", .color = {1, 1, 1, 0}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Game::LANE1, Game::JUDGE_LEVEL - 50, 0, 0, 0, 1,1,1});
    tm->create_entity<Game::Rhythm::KeyText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::KeyText(),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "D", .color = {1, 1, 1, 0}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Game::LANE2, Game::JUDGE_LEVEL - 50, 0, 0, 0, 1,1,1});
    tm->create_entity<Game::Rhythm::KeyText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::KeyText(),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "L", .color = {1, 1, 1, 0}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Game::LANE3, Game::JUDGE_LEVEL - 50, 0, 0, 0, 1, 1,1});
    tm->create_entity<Game::Rhythm::KeyText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Rhythm::KeyText(),
        Game::Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = ";", .color = {1, 1, 1, 0}, .layer = 50},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Game::LANE4, Game::JUDGE_LEVEL - 50, 0, 0, 0, 1, 1, 1});
}

std::array speed_list2 = {2.5f, 3.0f, 4.0f}; // in case of preset speed

inline Game::Battle::RhythmState create_rhythm_state(const int level, const int note_count)
{
    int accept_gain;
    if (note_count > 0)
        accept_gain = 20000/note_count;
    else
        accept_gain = 1;
    Game::Battle::RhythmState state(1, accept_gain, note_count, speed_list2[level], speed_list2[level]);
    const int accept_loss1 = accept_gain*5;
    const int accept_loss2 = accept_gain*2;
    state.accept_loss.normal = accept_loss1;
    state.accept_loss.accent = accept_loss1;
    state.accept_loss.rain = accept_loss2;
    state.accept_loss.hold = accept_loss1;
    state.accept_loss.hold_end = accept_loss2;

    constexpr float full_accuracy = 10000.00f; // represent full 100.00%
    state.apn = full_accuracy / static_cast<float>(state.total_notes);
    return (state);
}

inline std::string get_dsl_path2(const int level)
{
    std::string dsl_path = "dsl/02-";
    switch (level)
    {
        case 0:
        return dsl_path.append("L.th0");
        case 1:
        return dsl_path.append("S.th0");
        case 2:
        return dsl_path.append("B.th0");
        default:
        return dsl_path.append("L.th0");
    }
}

int Scene::Level2::load_chart(
    const std::shared_ptr<TaskManager> &tm,
    Game::Battle::ChartData chart)
{
    using Timing = Game::Rhythm::Timing;
    using HoldStart = Game::Rhythm::HoldStart;
    using NoteType = Game::Rhythm::NoteType;
    using NoteStatus = Game::Rhythm::NoteStatus;

    int note_count = 0;
    // repeat for each lane
    LOG_INFO("Loading chart...");
    for (auto &lane: chart.lanes)
    {
        while (lane.current_note < lane.notes.size())
        {
            const auto &note = lane.notes.at(lane.current_note);
            const auto pos = lane_to_point(lane.lane_number);

            if (note.is_hold)
            {
                if (note.timing < note.timing_end)
                {
                    if (note.note_type != 2)
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

                        note_count += 2;
                    }
                    else
                    {
                        LOG_INFO("Create hold note error: Hold note cannot have rain type")
                    }
                }
                else
                {
                    LOG_INFO("Create hold note error: Timing end must be more than initial timing")
                }
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

                note_count += 1;
            }

            ++lane.current_note;
        }
    }
    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent{Game::Battle::LevelDiff},
        write_difficulty(chart.difficulty),
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{-Game::HALF_WIDTH * 37/40 - 10, Game::HALF_HEIGHT * 3/5, 0}, 0, 0, 0});
    LOG_INFO("Finished loading chart")
    LOG_INFO("Note count = %d", note_count)

    return note_count;
}

Scene::Level2 Scene::Level2::instance()
{
    static Level2 instance;
    return (instance);
}

std::vector diff_list2 = {
    Game::Battle::Difficulty(Game::Battle::LIGHT, 2),
    Game::Battle::Difficulty(Game::Battle::SPARK, 4),
    Game::Battle::Difficulty(Game::Battle::BLAZE, 5),
};

inline Game::Battle::LevelData create_level2_data()
{
    Game::Battle::BpmInfo bpm;
    constexpr std::array timing_list = {27000, 72000, 110500};
    for (int m : timing_list)
    {
        Game::Battle::BpmInfo::InfoPair info{};
        info.bpm = 170.00f;
        info.timing = m;
        bpm.bpm_list.emplace_back(info);
    }
    return Game::Battle::LevelData(
    "Strike Against The World!",
    "Pooh5821",
    170.00f,
    bpm,
    diff_list2, 139000
    );
}

std::shared_ptr<Scene::Level2::TaskManager> Scene::Level2::init()
{
    ResourceManager rm;
    const System::ECS::pid level_id = rm.reserve_process();
    rm.add_resource(level_id, create_level2_data());

    const System::ECS::pid battle_id = rm.reserve_process();
    rm.add_resource(battle_id, Game::Battle::BattleState(100,10000,diff_list2[0]));

    return init(rm);
}

std::shared_ptr<Scene::Level2::TaskManager> Scene::Level2::init([[maybe_unused]] ResourceManager &data)
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});
    if (data.query<Game::World::SaveState>().begin() != data.query<Game::World::SaveState>().end())
    {
        tm->create_entity<Game::World::SaveState>(std::move(data.query<Game::World::SaveState>().front()));
    }

    init_graphics(tm);

    const Game::Battle::BattleState bt_state = data.query<Game::Battle::BattleState>().front();
    const int level = bt_state.difficulty.difficulty;
    Game::BulletHell::BulletScript script{"dsl/ShotData.th0",get_dsl_path2(level).c_str()};

    const int note_count = load_chart(tm, load_level_02_chart(level));

    tm->create_entity<Game::Battle::BattleState,
    Game::Battle::BulletHellState,
    Game::Battle::RhythmState,
    Game::Battle::BulletRegistry,
    Game::Battle::BulletLoader,
    Game::Battle::PatternContainer,
    Game::Render::AnimationDataRegistry,
    Game::Audio::SoundRegistry>
    (
        Game::Battle::BattleState{bt_state.max_hp, bt_state.max_accept_gauge, bt_state.difficulty},
        Game::Battle::BulletHellState(10),
        create_rhythm_state(level, note_count),
        std::move(script.bullet_registry),
        std::move(script.bullet_loader),
        std::move(script.pattern_container),
        init_anim_data(),
        Game::Audio::init_sounds(1));

    // InputManager
    tm->create_entity<Game::Input>(Game::Input());

    // Transition Data
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(27000, 1200, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(49700, 1100, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(72000, 1000, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(84500, 1200, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(110500, 1000, Game::Battle::RHYTHM));

    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(0));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(1));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(2));
    tm->create_entity<Game::Rhythm::Lane>(Game::Rhythm::Lane(3));

    tm->create_entity<Game::Battle::LevelData>(std::move(data.query<Game::Battle::LevelData>().front()));

    init_battle_components(tm);

    return (tm);
}

Scene::Level2::ResourceManager Scene::Level2::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting Level2 Scene.");
    return ResourceManager();
}