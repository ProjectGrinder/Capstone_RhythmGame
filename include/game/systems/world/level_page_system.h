#pragma once

#include "utils/constant.h"
namespace Game::World
{
    template<typename T>
    void create_level_page(T &syscall, LevelNodeEvent& level_node, const Battle::LevelData& data)
    {
        level_node.level_node_texts_pid.clear();
        int id = level_node.id;
        if (id >= LEVEL_NODE_POS.size())
            id = static_cast<int>(LEVEL_NODE_POS.size())-1;
        const auto pos = LEVEL_NODE_POS[id];
        level_node.level_node_box_pid = syscall.template create_entity<Render::Sprite, Render::Material, Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-HALF_WIDTH/2, HALF_HEIGHT*1/2, 0}, {HALF_WIDTH/2, HALF_HEIGHT*1/2, 0}, {HALF_WIDTH/2, -HALF_HEIGHT*1/2, 0}, {-HALF_WIDTH/2, -HALF_HEIGHT*1/2, 0}}, .layer = 45},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x,pos.y, 0, 0, 0, 0,0, 1},
            Render::Resize{{1, 1}, 500}
            );
        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = data.title, .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x-50, pos.y + HALF_HEIGHT*1/2 - 100, 0, 0, 0, 0,0, 1},
            Render::Resize{{1.5f, 1.5f}, 500}));

        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = data.artist_name, .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x, pos.y + HALF_HEIGHT*1/2 - 175, 0, 0, 0, 0,0, 1},
            Render::Resize{{1.2f, 1.2f}, 500}));

        level_node.level_node_texts_pid.push_back(
                syscall.template create_entity<Render::Text, Render::Material, Render::Transform>(
                        Render::Text{
                                .font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")),
                                .text = "BPM : " + std::to_string(static_cast<int>(data.main_bpm)),
                                .color = Math::Color{0, 0, 0, 1},
                                .layer = 51,
                                .align = Render::Center},
                        Render::Material(
                                get_assets_record_ptr(get_assets_id("sprite_vs")),
                                get_assets_record_ptr(get_assets_id("sprite_ps"))),
                        Render::Transform{pos.x, pos.y + HALF_HEIGHT * 1/2 - 250, 0, 0, 0, 0, 0, 1},
                        Render::Resize{{1, 1}, 500}));

        const size_t diff_len = data.difficulties.size();
        for (int i=0;i<diff_len;i++)
        {
            const std::string diff_name[4] = {"LIGHT", "SPARK", "BLAZE", "ASTRA"};
            const Battle::Difficulty diff = data.difficulties[i];
            level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = diff_name[diff.difficulty] + " " + std::to_string(diff.level), .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x - HALF_WIDTH/2 + HALF_WIDTH * (float)(i + 1) / (float)(diff_len+1), pos.y - HALF_HEIGHT*1/2 + 150, 0, 0, 0, 0,0, 1},
            Render::Resize{{1.f, 1.f}, 500}));
        }

        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>(
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Exit", .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x,pos.y - HALF_HEIGHT*1/2 + 50, 0, 0, 0, 0,0, 1},
            Render::Resize{{0.8f, 0.8f}, 500}));

        level_node.select_rect_pid = syscall.template create_entity<Render::Sprite, Render::Material, Render::Transform>(
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-100, 40, 0}, {100, 40, 0}, {100, -40, 0}, {-100, -40, 0}}, .color = {0,0.5f,1,0.5f}, .layer = 50},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{pos.x - HALF_WIDTH/2 + HALF_WIDTH * 1.f / (float)(diff_len+1),pos.y - HALF_HEIGHT*1/2 + 160, 0, 0, 0, 0,0, 1},
            Render::Resize{{1, 1}, 500});
    }

    template<typename T>
    void destroy_level_node(T &syscall, System::ECS::pid ent, const LevelNodeEvent &level_node, EventState &event_state)
    {
        syscall.template add_component<BulletHell::Particle>(
                level_node.level_node_box_pid, BulletHell::Particle(100, BulletHell::Shrink, 500));
        for (System::ECS::pid pid: level_node.level_node_texts_pid)
        {
            syscall.template add_component<BulletHell::Particle>(
                    pid, BulletHell::Particle(100, BulletHell::Shrink, 500));
        }
        syscall.template add_component<BulletHell::Particle>(
                level_node.select_rect_pid, BulletHell::Particle(100, BulletHell::Shrink, 500));
        syscall.template remove_component<LevelNodeEvent>(ent);
        event_state.has_event = false;
    }

    template<typename T>
    void adjust_option_rect(T &syscall, System::ECS::pid id,
            const uint16_t level_id, const int sel, const int diff, const size_t diff_len,
            Render::Transform &tra, Render::Sprite &spr)
    {
        Math::Point pos{};
        if (level_id < LEVEL_NODE_POS.size())
            pos = LEVEL_NODE_POS[static_cast<int>(level_id)];
        else
            pos = LEVEL_NODE_POS[LEVEL_NODE_POS.size()-1];
        // Test if viable
        syscall.template remove_component<Render::Resize>(id);
        tra.scaleX = 0;
        syscall.template add_component<Render::Resize>(id, Render::Resize({1,1},500));
        if (sel == 0)
        {
            tra.position = {pos.x,pos.y - HALF_HEIGHT*1/2 + 60};
            spr.color = Math::Color{0.5f,0.5f,0.5f,0.5f};
        }
        else
        {
            tra.position = { pos.x-HALF_WIDTH/2 + HALF_WIDTH * (float)(diff + 1) / (float)(diff_len+1),pos.y-HALF_HEIGHT*1/2 + 160 };
            Math::Color box_color;
            if (diff <= 3)
                box_color = DIFF_COLOR[diff];
            else
                box_color = Math::Color{0.5f,0.5f,0.5f};
            box_color.a = 0.5f;
            spr.color = box_color;
        }
    }

    template<typename T>
    void level_node_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<LevelRegistry> &level_reg_query,
            System::ECS::Query<GlobalState> &global_query,
            System::ECS::Query<EventState, LevelNodeEvent> &query1,
            System::ECS::Query<Audio::SoundRegistry> &audio_query)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();
        const auto &level_registry = level_reg_query.front().components.get<LevelRegistry>();
        auto &global = global_query.front().components.get<GlobalState>();
        const auto &sound_reg = audio_query.front().components.get<Audio::SoundRegistry>();
        auto sounds = sound_reg.audios;

        for (auto &[id, comps] : query1)
        {
            auto &level_node = comps.get<LevelNodeEvent>();
            const auto &level_data = level_registry.level_datas[level_node.id];

            if (level_node.level_node_box_pid == INVALID_PID)
            {
                global.level_selected = level_node.id;
                Audio::audio_play(sounds["sound_popup"]);
                create_level_page(syscall, level_node, level_data);
            }

            auto &level_info = level_registry.level_datas[level_node.id];
            if (input.escape_pressed || ((input.enter_pressed || input.z_pressed) && level_node.selection == 0))
            {
                Audio::audio_play(sounds["sound_popup"]);
                destroy_level_node(syscall,id, level_node, comps.get<EventState>());
                global.level_selected = -1;
                global.diff_selected = -1;
            }


            if ((input.enter_pressed || input.z_pressed) && level_node.selection == 1)
            {
                global.diff_selected = level_node.diff;
                Audio::audio_play(sounds["sound_confirm"]);
                Scene::switch_to_level(level_node.id);
                destroy_level_node(syscall,id, level_node, comps.get<EventState>());
            }

            if (input.up_pressed || input.down_pressed)
            {
                Audio::audio_play(sounds["sound_select"]);
                level_node.selection = (level_node.selection+1)%2;
                adjust_option_rect(syscall, level_node.select_rect_pid, level_node.id, level_node.selection, level_node.diff,
                    level_info.difficulties.size(), syscall.template query<Render::Transform>(level_node.select_rect_pid), syscall.template query<Render::Sprite>(level_node.select_rect_pid));
            }
            if (input.left_pressed || input.right_pressed)
            {
                level_node.selection = 1;
                Audio::audio_play(sounds["sound_select"]);
                const auto count = static_cast<uint8_t>(level_info.difficulties.size());
                level_node.diff = static_cast<uint8_t>((static_cast<int>(level_node.diff) + Physics::sign(input.right_pressed) + count) % count);
                adjust_option_rect(syscall, level_node.select_rect_pid, level_node.id, level_node.selection, level_node.diff,
                    level_info.difficulties.size(), syscall.template query<Render::Transform>(level_node.select_rect_pid), syscall.template query<Render::Sprite>(level_node.select_rect_pid));
            }
        }
    }
} // namespace Game::Overview

