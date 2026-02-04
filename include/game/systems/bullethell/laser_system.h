#pragma once

namespace Game::BulletHell
{
    // Default FadeIn/Out
    template <typename T>
    void Laser_System([[maybe_unused]] T &syscall, System::ECS::Query<Laser, Particle, Delay, Physics::Scale, Position, Rotation, Render::Material>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        constexpr auto frame_time = 1;
        for (auto &[id, comps] : query)
        {
            auto &laser = comps.get<Booming>();
            const auto &particle = comps.get<Particle>();
            auto &delay_comp = comps.get<Delay>();
            auto &pos = comps.get<Position>();
            auto &scl = comps.get<Physics::Scale>();
            auto &render = comps.get<Render::Material>();
            const auto angleZ = comps.get<Physics::Rotation>().angleZ * std::acos(0.0f)/90.0f;

            if (!laser.is_activated)
            {
                //Change after frame time. This logic sucks.
                if (delay_comp.delay > 1)
                {
                    scl.scaleX = laser.length;
                    pos.x = laser.start_pos_x + scl.scaleX*cos(angleZ)/2;
                    pos.y = laser.start_pos_y + scl.scaleX*sin(angleZ)/2;
                    render.color.a = 0.25;
                }
                else if (delay_comp.delay <= 0)
                {
                    scl.scaleX += laser.speed * frame_time;
                    if (scl.scaleX >= laser.length)
                    {
                        scl.scaleX = laser.length;
                        laser.is_activated = true;
                        delay_comp.delay = particle.lifetime - static_cast<int>(ceil(laser.length/laser.speed)) - 1;
                        delay_comp.delay = delay_comp.delay > 0 ? delay_comp.delay : 0;
                    }
                    pos.x = laser.start_pos_x + scl.scaleX*cos(angleZ)/2;
                    pos.y = laser.start_pos_y + scl.scaleX*sin(angleZ)/2;

                }
                else
                {
                    scl.scaleX = 0;
                    pos.x = laser.start_pos_x;
                    pos.y = laser.start_pos_y;
                    render.color.a = 1;
                }
            }
            else
            {
                if (delay_comp.delay <= 0)
                {
                    scl.scaleX -= laser.speed * frame_time;
                    pos.x += laser.speed * cos(angleZ) * frame_time;
                    pos.y += laser.speed * sin(angleZ) * frame_time;
                }
            }
        }
    }
}