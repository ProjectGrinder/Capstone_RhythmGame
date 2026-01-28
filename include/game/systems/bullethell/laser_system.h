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
            const auto &rot = comps.get<Rotation>();
            auto &scl = comps.get<Physics::Scale>();
            auto &render = comps.get<Render::Material>();

            auto max_pos_x = laser.start_pos_x + scl.scaleY*cos(rot.angleZ);
            auto max_pos_y = laser.start_pos_y + scl.scaleY*sin(rot.angleZ);
            if (laser.lifetime < 0)
            {
                if (scl.scaleX > 0)
                {
                    // translate
                    pos.x = (max_pos_x - scl.scaleY*cos(rot.angleZ))/2;
                    pos.y = (max_pos_y - scl.scaleY*cos(rot.angleZ))/2;
                    scl.scaleY -= laser.speed * frame_time;
                }
                else
                {
                    // Destroy entity? ObjectPool? Later
                }
            }
            else
            {
                //Change after frame time
                if (delay_comp.delay > 0)
                {
                    pos.x = (max_pos_x - laser.start_pos_x)/2;
                    pos.y = (max_pos_y - laser.start_pos_x)/2;
                    scl.scaleY = laser.length;
                    render.color.a = 0.25;
                }

                if (delay_comp.delay <= 0)
                {
                    if (render.color.a != 1)
                    {
                        pos.x = laser.start_pos_x;
                        pos.y = laser.start_pos_y;
                        scl.scaleY = 0;
                        render.color.a = 1;
                    }
                    if (scl.scaleY < laser.length)
                    {
                        // translate
                        scl.scaleY += laser.speed * frame_time;
                        pos.x = (laser.start_pos_x + scl.scaleY*cos(rot.angleZ))/2;
                        pos.y = (laser.start_pos_y + scl.scaleY*cos(rot.angleZ))/2;
                    }
                    laser.lifetime -= frame_time;
                }
            }

        }
    }
}