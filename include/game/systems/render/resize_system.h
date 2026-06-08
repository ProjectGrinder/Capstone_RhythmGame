#pragma once

namespace Game::Render
{
    template<typename T>
    void resize_system(
            [[maybe_unused]] T &syscall,
            System::ECS::Query<Resize, Transform> &query)
    {
        for (auto &[id, comps] : query)
        {
            auto &resize_c = comps.get<Resize>();
            if (resize_c.current_time >= resize_c.time)
            {
                syscall.template remove_component<Resize>(id);
                continue;
            }
            comps.get<Transform>().scaleX = Physics::lerp_at_frame(comps.get<Transform>().scaleX, resize_c.scale.x, resize_c.time);
            comps.get<Transform>().scaleY = Physics::lerp_at_frame(comps.get<Transform>().scaleY, resize_c.scale.y, resize_c.time);
            comps.get<Transform>().scaleZ = Physics::lerp_at_frame(comps.get<Transform>().scaleZ, resize_c.scale.z, resize_c.time);
            resize_c.current_time += static_cast<int>(get_delta_time());
        }
    }
} // namespace Game::Overview
