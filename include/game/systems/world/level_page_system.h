#pragma once

#include "utils/constant.h"
namespace Game::World
{
    template<typename T>
    void create_level_page(T &syscall, LevelNodeEvent& level_node, const Battle::LevelData& data)
    {
        level_node.level_node_texts_pid.clear();
        level_node.level_node_box_pid = syscall.template create_entity<Render::Sprite, Render::Material, Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-HALF_WIDTH/2, HALF_HEIGHT*3/4, 0}, {HALF_WIDTH/2, HALF_HEIGHT*3/4, 0}, {HALF_WIDTH/2, -HALF_HEIGHT*3/4, 0}, {-HALF_WIDTH/2, -HALF_HEIGHT*3/4, 0}}, .layer = 45},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, 0, 0, 0, 0, 0,0, 1},
            Render::Resize{{1, 1}, 500}
            );
        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = data.title, .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, HALF_HEIGHT*3/4 - 100, 0, 0, 0, 0,0, 1},
            Render::Resize{{1.5f, 1.5f}, 500}));

        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = data.artist_name, .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, HALF_HEIGHT*3/4 - 150, 0, 0, 0, 0,0, 1},
            Render::Resize{{0.75f, 0.75f}, 500}));

        level_node.level_node_texts_pid.push_back(
                syscall.template create_entity<Render::Text, Render::Material, Render::Transform>(
                        Render::Text{
                                .font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")),
                                .text = "main bpm : " + std::to_string(data.main_bpm),
                                .color = Math::Color{0, 0, 0, 1},
                                .layer = 51,
                                .align = Render::Center},
                        Render::Material(
                                get_assets_record_ptr(get_assets_id("sprite_vs")),
                                get_assets_record_ptr(get_assets_id("sprite_ps"))),
                        Render::Transform{0, HALF_HEIGHT * 3 / 4 - 200, 0, 0, 0, 0, 0, 1},
                        Render::Resize{{0.75f, 0.75f}, 500}));

        const size_t diff_len = data.difficulties.size();
        for (int i=0;i<diff_len;i++)
        {
            const std::string diff_name[4] = {"LIGHT", "SPARK", "BLAZE", "ASTRA"};
            const Battle::Difficulty diff = data.difficulties[i];
            level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = diff_name[diff.difficulty] + "(" + std::to_string(diff.level) + ")", .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{-HALF_WIDTH/2 + HALF_WIDTH * (float)(i + 1) / (float)(diff_len+1), -HALF_HEIGHT*3/4 + 120, 0, 0, 0, 0,0, 1},
            Render::Resize{{1.f, 1.f}, 500}));
        }

        level_node.level_node_texts_pid.push_back(syscall.template create_entity<Render::Text, Render::Material, Render::Transform>(
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = "Exit", .color = Math::Color{0, 0, 0, 1}, .layer = 51, .align = Render::Center},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0,-HALF_HEIGHT*3/4 + 50, 0, 0, 0, 0,0, 1},
            Render::Resize{{0.8f, 0.8f}, 500}));

        level_node.select_rect_pid = syscall.template create_entity<Render::Sprite, Render::Material, Render::Transform>(
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-100, 40, 0}, {100, 40, 0}, {100, -40, 0}, {-100, -40, 0}}, .color = {0.75f,0.25f,0.25f,0.5f}, .layer = 50},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{-HALF_WIDTH/2 + HALF_WIDTH * 1.f / (float)(diff_len+1),-HALF_HEIGHT*3/4 + 120, 0, 0, 0, 0,0, 1},
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
    void adjust_option_rect(T &syscall, System::ECS::pid id, const int sel, const int diff, const size_t diff_len, Render::Transform &tra)
    {
        // Test if viable
        syscall.template remove_component<Render::Resize>(id);
        tra.scaleX = 0;
        syscall.template add_component<Render::Resize>(id, Render::Resize({1,1},500));
        if (sel == 0) tra.position = {0,-HALF_HEIGHT*3/4 + 50};
        else tra.position = { -HALF_WIDTH/2 + HALF_WIDTH * (float)(diff + 1) / (float)(diff_len+1),-HALF_HEIGHT*3/4 + 120 };
    }

    template<typename T>
    void level_node_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<LevelRegistry> &level_reg_query,
            System::ECS::Query<EventState, LevelNodeEvent> &query1)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();
        const auto &level_registry = level_reg_query.front().components.get<LevelRegistry>();

        for (auto &[id, comps] : query1)
        {
            auto &level_node = comps.get<LevelNodeEvent>();
            const auto &level_data = level_registry.level_datas[level_node.id];

            if (level_node.level_node_box_pid == INVALID_PID) create_level_page(syscall, level_node, level_data);

            auto &level_info = level_registry.level_datas[level_node.id];
            if (input.x_pressed || input.escape_pressed || (input.z_pressed && level_node.selection == 0))
                destroy_level_node(syscall,id, level_node, comps.get<EventState>());

            if (input.z_pressed && level_node.selection == 1)
            {
                Scene::queue_change_scene<Scene::Level1>();
                destroy_level_node(syscall,id, level_node, comps.get<EventState>());
            }

            if (input.up_pressed || input.down_pressed)
            {
                level_node.selection = (level_node.selection+1)%2;
                adjust_option_rect(syscall, level_node.select_rect_pid, level_node.selection, level_node.diff, level_info.difficulties.size(), syscall.template query<Render::Transform>(level_node.select_rect_pid));
            }
            if (input.left_pressed || input.right_pressed)
            {
                level_node.selection = 1;
                level_node.diff = (level_node.diff + Physics::sign(input.right_pressed)) % (uint8_t)level_info.difficulties.size();
                adjust_option_rect(syscall, level_node.select_rect_pid, level_node.selection, level_node.diff, level_info.difficulties.size(), syscall.template query<Render::Transform>(level_node.select_rect_pid));
            }
        }
    }
} // namespace Game::Overview

