#pragma once
#include "utils/constant.h"
namespace Game::World
{
    template<typename T>
    void create_level_page(T &syscall, LevelNode& level_node, Battle::LevelData& data)
    {
        syscall.template create_entity<Render::Sprite, Render::Material, Render::Transform>
        (
            Render::Sprite{.sp = get_assets_record_ptr(get_assets_id("Square")), .pos = {{-HALF_WIDTH/2, HALF_HEIGHT*3/4, 0}, {HALF_WIDTH/2, HALF_HEIGHT*3/4, 0}, {HALF_WIDTH/2, -HALF_HEIGHT*3/4, 0}, {-HALF_WIDTH/2, -HALF_HEIGHT*3/4, 0}}, .layer = 50},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{0, 0, 0, 0, 0, 0,0, 1},
            Render::Resize{{1, 1}, 500}
            );
        syscall.template create_entity<Render::Text, Render::Material, Render::Transform>
        (
            Render::Text{.font = get_assets_record_ptr(get_assets_id("Klub04TT-NoBG")), .text = data.title, .color = Math::Color{0, 0, 0, 1}, .layer = 51},
            Render::Material(get_assets_record_ptr(get_assets_id("sprite_vs")), get_assets_record_ptr(get_assets_id("sprite_ps"))),
            Render::Transform{Math::Point{0, 100, 0}, 0, 0, 0});
    }


    template<typename T>
    void level_node_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Input> &input_query,
            System::ECS::Query<LevelRegistry> &level_reg_query,
            System::ECS::Query<EventState, LevelNodeEvent> &query1,
            System::ECS::Query<LevelNode> &query2)
    {
        if (input_query.begin() == input_query.end())
            return;

        const auto &input = input_query.front().components.get<Input>();
        // const auto &level_registry = level_reg_query.front().components.get<LevelRegistry>();

        for (auto &[id, comps] : query1)
        {
            // auto &event_state = comps.get<EventState>();
            auto &level_node_ev = comps.get<LevelNodeEvent>();

            if (level_node_ev.level_node_id != INVALID_ID)
            {
                // TODO : More Render comps to add
                level_node_ev.level_node_id = syscall.create_entity(LevelNode(level_node_ev.id));
            }

            else
            {
                // No ID query access so this.
                for (auto &[id2, comps2] : query2)
                {
                    if (level_node_ev.level_node_id == id2)
                    {
                        auto &level_node = comps2.get<LevelNode>();
                        if (input.x_pressed || input.escape_pressed || (input.z_pressed && level_node.sel == LevelNode::Selection::Cancel))
                        {
                            syscall.template remove_component<LevelNodeEvent>(id);
                            // Destroy Tab
                        }

                        if (input.z_pressed && level_node.sel == LevelNode::Selection::Confirm)
                        {
                            // TODO : Change scene
                        }

                        if (input.up_pressed || input.down_pressed)
                        {
                            level_node.sel = static_cast<LevelNode::Selection>((static_cast<int>(level_node.sel)+1)%static_cast<int>(LevelNode::Selection::Count));
                        }
                    }
                }
            }
        }
    }
} // namespace Game::Overview

