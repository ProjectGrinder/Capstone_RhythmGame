#pragma once

#include "scene.h"
#include "system.h"

#include "game.h"

void init_graphics(const std::shared_ptr<Scene::Level1::TaskManager>& tm)
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
    load_sprite("img/rhythm/note_border.dds", "note_border", 200, 40);
    load_sprite("img/rhythm/judge_text.dds", "judge", 1280, 640);

    load_sprite("img/return.dds", "return", 1280, 720);

    load_sprite("img/level1_bg.dds", "level1_bg", 3840, 2160);
}

Game::Battle::PatternContainer create_pattern_container2()
{
    int t_beat = 447;
    using namespace Game::Battle;
    const std::vector<PatternStep> demo_step = {
        PatternStep(t_beat/2, OP_SET, 0b1110, 300, -90 ,0),
        PatternStep(t_beat/2, OP_SET, 0b0010, -5000),

};
    const std::vector<PatternSequence> demo_pattern = {
        PatternSequence(false, 0),
        // PatternSequence(false, 4),

    };
    auto demo_pattern_container = PatternContainer(demo_step,demo_pattern);
    return { PatternContainer(demo_pattern_container) };
}

Game::Battle::BulletLoader Scene::Level1::create_bullet_test()
{
    using namespace Game::Battle;
    using namespace Game::Physics;

    BulletLoader loader;

    int t_beat = 447;
    float b_offset = 0;
    constexpr float time_per_beat = 60000.f / 134.f;
    auto beat_time = [](const float beat)
    {
        return static_cast<int>(std::round(beat * time_per_beat));
    };

    const auto box_left = -Game::BOX_SIZE + Game::HALF_WIDTH / 32;
    const auto box_right = Game::BOX_SIZE - Game::HALF_WIDTH / 32;
    const auto box_up = Game::BOX_SIZE + Game::HALF_HEIGHT / 32 - Game::HALF_HEIGHT/ 3;
    const auto box_down = -Game::BOX_SIZE-Game::HALF_HEIGHT/32-Game::HALF_HEIGHT/3;

    for (int i=0; i < 16; i++ , b_offset+=0.5f)
    {
        int i8 = i%8;
        float d = (i8 / 4 == 1) ? (float)(8 - i8 -1) : (float)i8;
        loader.CreateBullet(beat_time(b_offset),BulletData(box_left + (box_right-box_left)*d/4 + rand_float(-50,50), box_up, 400, 90, -1000,0, 1,0,5000,12));
        loader.CreateBullet(beat_time(b_offset + 0.25f),BulletData(box_right - (box_right-box_left)*d/4 + rand_float(-50,50), box_up+100, 400, 90, -1000,0, 1,0,5000,13));
    }
    for (int i=0; i < 15; i++, b_offset+=2.f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;
        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }
    }


    b_offset = 116;
    for (int i=0; i < 3; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+0.f), BulletData(box_left+100 , box_down+250, 0, 0,t_beat*2,t_beat*3,172));
            loader.CreateBullet(beat_time(b_offset+0.5f), BulletData(box_right-100 , box_down+250, 0, 0,t_beat*2,t_beat*3,172));
            loader.CreateBullet(beat_time(b_offset+1.f), BulletData(box_left+100 , box_down+500, 0, 0,t_beat*2,t_beat*3,172));
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+1.5f), BulletData(box_right-100 , box_down+500, 0, 0,t_beat*2,t_beat*3,172));
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 4; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+0.f), BulletData(Game::HALF_WIDTH, box_down + 100, 0, 180, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+0.33f), BulletData(-Game::HALF_WIDTH, box_down + 200, 0, 0, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+0.67f), BulletData(Game::HALF_WIDTH, box_down + 300, 0, 180, t_beat*2,t_beat*3, 164));
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+1.f), BulletData(-Game::HALF_WIDTH, box_down + 400, 0, 0, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+1.33f), BulletData(Game::HALF_WIDTH, box_down + 500, 0, 180, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+1.67f), BulletData(-Game::HALF_WIDTH, box_down + 600, 0, 0, t_beat*2,t_beat*3, 164));
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 3; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==1 || i==2)
        {
            for (int tof = 0; tof < 8; tof++)
            {
                loader.CreateBullet(beat_time(b_offset) + 40*tof,BulletData(0 , box_up, 500, static_cast<float>(-155 + tof*15),0,5000,100));
            }
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==1 || i==2)
        {
            for (int tof = 0; tof < 8; tof++)
            {
                loader.CreateBullet(beat_time(b_offset+1) + 40*tof,BulletData(0 , box_up, 500, static_cast<float>(-15 - tof*15),0,5000,100));
            }
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 2; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;
        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }
    }

    b_offset = 180;
    loader.CreateBullet(beat_time(b_offset),BulletData(box_left + (box_right-box_left)/2, box_down + (box_up-box_down)/2, 0, 0, t_beat*2,t_beat*3, 176));
    for (int i=0;i<480;i++, b_offset+=0.125f)
    {
        loader.CreateBullet(beat_time(b_offset),BulletData(box_left + (box_right-box_left)/2, box_down + (box_up-box_down)/2, 300, (float)i*42.f,0,5000,53));
        if (i>=192 && i%16==0)
        {
            const float x = i % 32 == 0 ? box_left + 100 : box_right - 100;
            const float y = (i%64/32)==0? box_down+250 : box_down+500;
            loader.CreateBullet(beat_time(b_offset), BulletData(x , y, 0, 0,t_beat*2,t_beat*3,172));
        }
        if (i>=256 && i%8==4)
        {
            loader.CreateBullet(beat_time(b_offset),BulletData(rand_float(box_left,box_right), box_up, 400, 90, -1000,0, 1,0,5000,144));
        }
    }

    b_offset = 244;
    for (int i=0; i < 3; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+0.f), BulletData(box_left+100 , box_down+250, 0, 0,t_beat*2,t_beat*3,172));
            loader.CreateBullet(beat_time(b_offset+0.5f), BulletData(box_right-100 , box_down+250, 0, 0,t_beat*2,t_beat*3,172));
            loader.CreateBullet(beat_time(b_offset+1.f), BulletData(box_left+100 , box_down+500, 0, 0,t_beat*2,t_beat*3,172));
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+1.5f), BulletData(box_right-100 , box_down+500, 0, 0,t_beat*2,t_beat*3,172));
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 4; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+0.f), BulletData(Game::HALF_WIDTH, box_down + 100, 0, 180, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+0.33f), BulletData(-Game::HALF_WIDTH, box_down + 200, 0, 0, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+0.67f), BulletData(Game::HALF_WIDTH, box_down + 300, 0, 180, t_beat*2,t_beat*3, 164));
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==2)
        {
            loader.CreateBullet(beat_time(b_offset+1.f), BulletData(-Game::HALF_WIDTH, box_down + 400, 0, 0, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+1.33f), BulletData(Game::HALF_WIDTH, box_down + 500, 0, 180, t_beat*2,t_beat*3, 164));
            loader.CreateBullet(beat_time(b_offset+1.67f), BulletData(-Game::HALF_WIDTH, box_down + 600, 0, 0, t_beat*2,t_beat*3, 164));
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 3; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;

        if (i==1 || i==2)
        {
            for (int tof = 0; tof < 8; tof++)
            {
                loader.CreateBullet(beat_time(b_offset) + 40*tof,BulletData(0 , box_up, 500, static_cast<float>(-155 + tof*15),0,5000,100));
            }
        }

        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }

        if (i==1 || i==2)
        {
            for (int tof = 0; tof < 8; tof++)
            {
                loader.CreateBullet(beat_time(b_offset+1) + 40*tof,BulletData(0 , box_up, 500, static_cast<float>(-15 - tof*15),0,5000,100));
            }
        }
    }

    b_offset+=2.f;

    for (int i=0; i < 2; i++, b_offset+=2.0f)
    {
        float randX;
        if (i%2==0) randX = box_left + rand_float(50,200);
        else randX = box_right - rand_float(50,200);
        float randAcc = -rand_float(1000,5000);
        loader.CreateBullet(beat_time(b_offset),BulletData(randX , -Game::HALF_HEIGHT, 10000, 90, randAcc,0,0,t_beat,25));
        float bomb_y = -Game::HALF_HEIGHT/4 + randAcc/20;
        for (int j=0; j < 8; j++)
        {
            loader.CreateBullet(beat_time(b_offset+1),BulletData(randX , bomb_y, 400, (float)j*45, 0,0,0,5000,1));
        }
    }

    return (loader);
}

std::vector diff_list = {
    Game::Battle::Difficulty(Game::Battle::LIGHT, 1),
    Game::Battle::Difficulty(Game::Battle::SPARK, 3),
    Game::Battle::Difficulty(Game::Battle::BLAZE, 5),
};

std::array total_note_list = {100, 150, 279}; // store total notes here

inline Game::Battle::RhythmState create_rhythm_state(const int level)
{
    Game::Battle::RhythmState state(1, 10, total_note_list[level], 4.0f, 4.0f);
    state.accept_loss.normal = 50;
    state.accept_loss.accent = 50;
    state.accept_loss.rain = 20;
    state.accept_loss.hold = 50;
    state.accept_loss.hold_end = 20;

    constexpr float full_accuracy = 10000.00f; // represent full 100.00%
    state.apn = full_accuracy / static_cast<float>(state.total_notes);
    return (state);
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
    diff_list, 142000
    );
}

void Scene::Level1::load_chart(
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

Scene::Level1 Scene::Level1::instance()
{
    static Level1 instance;
    return (instance);
}

// Game::Battle::Difficulty Scene::Level1::set_difficulty(const int level)
// {
//     switch (level)
//     {
//     case 0:
//         return Game::Battle::Difficulty(Game::Battle::LIGHT, 1);
//     case 1:
//         return Game::Battle::Difficulty(Game::Battle::SPARK, 3);
//     case 2:
//         return Game::Battle::Difficulty(Game::Battle::BLAZE, 5);
//     default:
//         return Game::Battle::Difficulty(Game::Battle::LIGHT, 1);
//     }
// }

std::shared_ptr<Scene::Level1::TaskManager> Scene::Level1::init()
{
    auto tm = std::make_shared<TaskManager>();
    tm->create_entity(Game::Render::Camera2D{.offset = {}, .scaleX = 1920, .scaleY = 1080, .rotation = 0});

    init_graphics(tm);

    constexpr int level = 2;

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
        Game::Battle::BattleState(200, total_note_list[level]*5, diff_list[level]),
        Game::Battle::BulletHellState(10),
        create_rhythm_state(level),
        read_bullet_data_from_file("ShotData.txt"),
        create_bullet_test(),
        create_pattern_container2(),
        init_anim_data(),
        Game::Audio::init_sounds(),
        Game::Rhythm::KeyInput(),
        Game::BulletHell::Input());

    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(16400, 1500, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(50149, 1500, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(65000, 1500, Game::Battle::RHYTHM));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(80000, 1500, Game::Battle::BULLET_HELL));
    tm->create_entity<Game::Battle::TransitionData>(Game::Battle::TransitionData(122500, 1000, Game::Battle::RHYTHM));

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
        Game::Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("normal")), .pos = {{-400, 10, 0}, {400, 10, 0}, {400, -10, 0}, {-400, -10, 0}},.color = {1,1,1,0}, .layer = 2},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{0, field.judge_level, 0}, 0, 0, 0}, {}
        );

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

    tm->create_entity<
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Render::Text{.font = font, .text = "ACCURACY", .color = Math::Color{0, 0, 0, 1}, .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{500, Game::HALF_HEIGHT * 4/5, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::UIComponent,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::UIComponent(Game::Battle::AccuracyText),
        Game::Render::Text{.font = font, .text = "100%", .color = Math::Color{0, 0, 0, 1}, .layer = 5},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{Math::Point{500, Game::HALF_HEIGHT * 3/4, 0}, 0, 0, 0});

    tm->create_entity<Game::Battle::TransitionText,
    Game::Render::Text,
    Game::Render::Material,
    Game::Render::Transform>
    (
        Game::Battle::TransitionText(),
        Game::Render::Text{.font = font, .text = "", .layer = 105},
        Game::Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
        Game::Render::Transform{0, Game::HALF_HEIGHT * 4/5, 0, 0, 0, 2.5f,2.5f,1});

    return (tm);
}

Scene::Level1::ResourceManager Scene::Level1::exit([[maybe_unused]] std::shared_ptr<TaskManager> &manager)
{
    LOG_INFO("Exiting Level1 Scene.");
    return ResourceManager();
}