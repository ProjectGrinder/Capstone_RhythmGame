#pragma once

namespace Game::BulletHell
{
    // Default FadeIn/Out
    template <typename T>
    void laser_system([[maybe_unused]] T &syscall, System::ECS::Query<Laser, Particle, Delay, Render::Transform, Rotation, Render::Material>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        for (auto &[id, comps] : query)
        {
            auto &laser = comps.get<Booming>();
            const auto &particle = comps.get<Particle>();
            auto &delay_comp = comps.get<Delay>();
            auto &tra = comps.get<Render::Transform>();
            auto &render = comps.get<Render::Material>();
            const auto angleZ = comps.get<Physics::Rotation>().angleZ * std::acos(0.0f)/90.0f;

            if (!laser.is_activated)
            {
                //Change after frame time. This logic sucks.
                if (delay_comp.delay > 1000)
                {
                    tra.scaleX = laser.length;
                    tra.position.x = laser.start_pos_x + tra.scaleX*cos(angleZ)/2;
                    tra.position.y = laser.start_pos_y + tra.scaleX*sin(angleZ)/2;
                    render.color.a = 0.25;
                }
                else if (delay_comp.delay <= 0)
                {
                    tra.scaleX += static_cast<float>(get_delta_time()/1000 * laser.length/laser.boom_frame);
                    if (tra.scaleX >= laser.length)
                    {
                        tra.scaleX = laser.length;
                        laser.is_activated = true;
                        delay_comp.delay = particle.lifetime - laser.boom_frame - 1;
                        delay_comp.delay = delay_comp.delay > 0 ? delay_comp.delay : 0;
                    }
                    tra.position.x = laser.start_pos_x + tra.scaleX*cos(angleZ)/2;
                    tra.position.y = laser.start_pos_y + tra.scaleX*sin(angleZ)/2;

                }
                else
                {
                    tra.scaleX = 0;
                    tra.position.x = laser.start_pos_x;
                    tra.position.y = laser.start_pos_y;
                    render.color.a = 1;
                }
            }
            else
            {
                if (delay_comp.delay <= 0)
                {
                    const auto laser_speed = static_cast<float>(get_delta_time()/1000 * laser.length/laser.boom_frame);
                    tra.scaleX -= laser_speed;
                    tra.position.x += laser_speed * cos(angleZ);
                    tra.position.y += laser_speed * sin(angleZ);
                }
            }
        }
    }
}