#pragma once
#include <fstream>
#include <regex>

// Currently unused -> Don't know how to read file here.

/*
inline void CalculateCollider(float *rect, int &p1)
{
    const int width = rect[2] - rect[0];
    const int height = rect[3] - rect[1];
    const int size = width < height ? width : height;
    const int hitbox = (size/3 - 3 > 3)? size/3 - 3 : size/3;
    p1 = hitbox;
}

inline void read_bullet_data_from_file(const char *filepath, Game::Battle::BulletRegistry &bullet_registry)
{
    std::ifstream f(filepath);
    std::string line;

    Game::Battle::BulletGraphicMap current;
    bool inBlock = false;

    while (std::getline(f, line))
    {
        line = std::regex_replace(line, std::regex("\\s+"), "");
        if (line.find("BulletData{") != std::string::npos) {
            current = Game::Battle::BulletGraphicMap{};
            inBlock = true;
            continue;
        }
        if (line.find("}") != std::string::npos)
        {
            bullet_registry.bulletGraphicMaps.push_back(current);
            inBlock = false;
            continue;
        }
        if (!inBlock) continue;

        // Necessary Info
        if (line.starts_with("rect"))
        {
            int r1, r2, r3, r4;
            sscanf(line.c_str(), "rect: ( %d , %d , %d , %d )", &r1, &r2, &r3, &r4);
            current.graphic_data = Game::Battle::GraphicData(r1, r2, r3, r4);
        }

        else if (line.starts_with("color"))
        {
            auto& gd = current.graphic_data;
            sscanf(line.c_str(), "color: ( %f , %f , %f , %f )", &gd.r, &gd.g, &gd.b, &gd.a);
        }

        else if (line.starts_with("sound"))
        {
            auto& gd = current.graphic_data;
            sscanf(line.c_str(), "sound: %d", &gd.bullet_spawn_sound);
        }

        else if (line.starts_with("sound"))
        {
            auto& gd = current.graphic_data;
            sscanf(line.c_str(), "sound: %d", &gd.bullet_spawn_sound);
        }

        else if (line.starts_with("bullet_type"))
        {
            float size;
            int frame;
            if (line.find("Booming") != std::string::npos)
            {
                sscanf(line.c_str(), "bullet_type: ( Booming , %f , %d )", &size, &frame);
                current.special_bullet_data = Game::Battle::SpecialBulletData(Game::Battle::Booming, size, frame);
            }

            if (line.find("Laser") != std::string::npos)
            {
                sscanf(line.c_str(), "bullet_type: ( Laser , %f , %d )", &size, &frame);
                current.special_bullet_data = Game::Battle::SpecialBulletData(Game::Battle::Laser, size, frame);
            }
        }

        else if (line.starts_with("collider"))
        {
            Game::Physics::ColliderType c;
            int p1,p2 = -1,p3=-1,p4=-1;
            int openB, closeB;
            int split = std::count(line.begin(), line.end(), ',');
            auto rect = current.graphic_data.src_rect;
            if (line.find("[") != std::string::npos)
            {
                c = Game::Physics::RECTANGLE;
                openB = line[line.find("[") + 1];
                closeB = line[line.find("]")];
            }

            else if (line.find("(") != std::string::npos)
            {
                c = Game::Physics::CIRCLE;
                openB = line[line.find("(") + 1];
                closeB = line[line.find(")")];
            }
            else
            {
                c = Game::Physics::CIRCLE;
                CalculateCollider(rect, p1);
                current.collider_data = Game::Battle::ColliderData(c, p1);
                continue;
            }

            std::string new_line = line.substr(openB, closeB-openB+1);
            if (new_line.size() > 2)
                CalculateCollider(rect, p1);
            if (split == 0)
                sscanf(new_line.c_str(), "%d", &p1);
            if (split == 1)
                sscanf(new_line.c_str(), "%d , %d", &p1, &p2);
            if (split == 3)
                sscanf(new_line.c_str(), "%d , %d , %d , %d", &p1, &p2, &p3, &p4);
            if (p2 == -1)
                current.collider_data = Game::Battle::ColliderData(c, p1);
            else if (p3 == -1)
                current.collider_data = Game::Battle::ColliderData(c, p1, p2);
            else
                current.collider_data = Game::Battle::ColliderData(c, p1, p2,p3,p4);
        }

        else if (line.starts_with("damage")) {
            sscanf(line.c_str(), "damage = %f", &current.damage_mul);
        }
        else if (line.starts_with("pierce")) {
            sscanf(line.c_str(), "pierce = %d", &current.pierce);
        }
        else if (line.starts_with("lifetime")) {
            sscanf(line.c_str(), "lifetime = %d", &current.lifetime);
            current.lifetime *= 1000/60;
        }

    }

}
*/
