#pragma once

namespace Game::BulletHell
{
    // Default FadeIn/Out
    template <typename T>
    void Laser_System([[maybe_unused]] T &syscall, System::ECS::Query<Laser,Delay, Physics::Scale, Position, Rotation, Render::Material>& query, System::ECS::Query<Battle::BattleState> &query2)
    {
        if (query2.begin() == query2.end())
            return;

        constexpr auto frame_time = 1;
        for (auto &[id, comps] : query)
        {
            auto &laser = comps.get<Booming>();
            const auto &delay_comp = comps.get<Delay>();
            auto &pos = comps.get<Position>();
            auto &scl = comps.get<Physics::Scale>();
            auto &render = comps.get<Render::Material>();
            const auto angleZ = comps.get<Physics::Rotation>().angleZ * std::acos(0.0f)/90.0f;
            if (laser.lifetime <= 0)
            {
                if (scl.scaleX > 0.02f)
                {
                    // translate
                    scl.scaleX -= laser.speed * frame_time;
                    pos.x += laser.speed * cos(angleZ) * frame_time;
                    pos.y += laser.speed * sin(angleZ) * frame_time;
                }
                else
                {
                    // Destroy entity? ObjectPool?
                    // --> Don't Destroy, it's particle job
                    scl.scaleX = 0;
                }
            }
            else
            {
                //Change after frame time
                if (delay_comp.delay > 1)
                {
                    scl.scaleX = laser.length;
                    pos.x = laser.start_pos_x + scl.scaleX*cos(angleZ)/2;
                    pos.y = laser.start_pos_y + scl.scaleX*sin(angleZ)/2;
                    render.color.a = 0.25;
                }

                else if (delay_comp.delay <= 0)
                {
                    if (scl.scaleX < laser.length)
                    {
                        // translate
                        scl.scaleX += laser.speed * frame_time;

                    }
                    else
                        scl.scaleX = laser.length;

                    pos.x = laser.start_pos_x + scl.scaleX*cos(angleZ)/2;
                    pos.y = laser.start_pos_y + scl.scaleX*sin(angleZ)/2;
                    laser.lifetime -= frame_time;
                }

                else
                {
                    scl.scaleX = 0;
                    pos.x = laser.start_pos_x;
                    pos.y = laser.start_pos_y;
                    render.color.a = 1;
                }


            }

        }
    }
}