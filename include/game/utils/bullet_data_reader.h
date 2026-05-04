#pragma once
#include <regex>
#include <sstream>

// Currently unused -> Don't know how to read file here.
// TODO : Migrate to source

extern "C"{
    typedef unsigned long       DWORD;
    DWORD file_read(_Out_ FileContent **content, const char *name);
    void file_free(FileContent **file);
}

inline void CalculateCollider(const int *rect, int &p1)
{
    int width = rect[2] - rect[0];
    int height = rect[3] - rect[1];
    int size = width < height ? width : height;
    int hitbox = size/2;
    p1 = hitbox;
}

inline void read_bullet_data_from_file(const char *filepath, Game::Battle::BulletRegistry &bullet_registry)
{
    FileContent* file_content;
    file_read(&file_content, filepath);

    std::string content(file_content->data, file_content->size);
    std::istringstream f(content);

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
            std::string new_line = line.substr(6, line.size()-6+1);
            size_t p1 = new_line.find(',');
            size_t p2 = new_line.find(',', p1 + 1);
            size_t p3 = new_line.find(',', p2 + 1);
            current.graphic_data = Game::Battle::GraphicData(
                std::stoi(new_line.substr(0, p1)),
                std::stoi(new_line.substr(p1 + 1, p2 - p1 - 1)),
                std::stoi(new_line.substr(p2 + 1, p3 - p2 - 1)),
                std::stoi(new_line.substr(p3 + 1)));

            int size;
            CalculateCollider(current.graphic_data.src_rect, size);
            current.collider_data = Game::Battle::ColliderData(Game::Physics::CIRCLE, static_cast<float>(size));
        }

        else if (line.starts_with("color"))
        {
            auto& gd = current.graphic_data;
            std::string new_line = line.substr(7, line.size()-7+1);
            size_t p1 = new_line.find(',');
            size_t p2 = new_line.find(',', p1 + 1);
            size_t p3 = new_line.find(',', p2 + 1);
            gd.r = std::stof(new_line.substr(0, p1));
            gd.g = std::stof(new_line.substr(p1 + 1, p2 - p1 - 1));
            gd.b = std::stof(new_line.substr(p2 + 1, p3 - p2 - 1));
            gd.a = std::stof(new_line.substr(p3 + 1));
        }

        else if (line.starts_with("sound"))
        {
            auto& gd = current.graphic_data;
            std::string inside = line.substr(7, line.size()-7+1);
            gd.bullet_spawn_sound = std::stoi(inside.substr(0));
        }


        else if (line.starts_with("bullet_type"))
        {
            std::string new_line = line.substr(12, line.size()-12+1);
            size_t p1 = new_line.find(',');
            size_t p2 = new_line.find(',', p1 + 1);
            if (line.find("Booming") != std::string::npos)
            {
                current.special_bullet_data = Game::Battle::SpecialBulletData(
                    Game::Battle::Booming,
                    std::stof(new_line.substr(p1 + 1, p2 - p1 - 1)),
                    std::stoi(new_line.substr(p2 + 1))
                    );
                current.pierce = 999;
            }

            if (line.find("Laser") != std::string::npos)
            {
                current.special_bullet_data = Game::Battle::SpecialBulletData(
                    Game::Battle::Laser,
                    std::stof(new_line.substr(p1 + 1, p2 - p1 - 1)),
                    std::stoi(new_line.substr(p2 + 1))
                    );
                current.pierce = 999;
            }
        }

        else if (line.starts_with("collider"))
        {
            Game::Physics::ColliderType c;
            int p1,p2 = -1,p3=-1,p4=-1;
            size_t openB;
            int split = static_cast<int>(std::count(line.begin(), line.end(), ','));
            auto rect = current.graphic_data.src_rect;
            if (line.find("[") != std::string::npos)
            {
                c = Game::Physics::RECTANGLE;
                openB = line.find("[") + 1;
            }

            else if (line.find("(") != std::string::npos)
            {
                c = Game::Physics::CIRCLE;
                openB = line.find("(") + 1;
            }
            else
            {
                continue;
            }

            std::string new_line = line.substr(openB, line.size()-openB+1);
            if (new_line.size() <= 2)
                CalculateCollider(rect, p1);
            else
            {
                if (split == 0)
                    p1 = std::stoi(new_line);
                else if (split == 1)
                {
                    size_t pp1 = new_line.find(',');
                    p1 = std::stoi(new_line.substr(0, pp1));
                    p2 = std::stoi(new_line.substr(pp1 + 1));
                }
                else if (split == 3)
                {
                    size_t pp1 = new_line.find(',');
                    size_t pp2 = new_line.find(',', pp1 + 1);
                    size_t pp3 = new_line.find(',', pp2 + 1);
                    p1 = std::stoi(new_line.substr(0, pp1));
                    p2 = std::stoi(new_line.substr(pp1 + 1, pp2 - pp1 - 1));
                    p3 = std::stoi(new_line.substr(pp2 + 1, pp3 - pp2 - 1));
                    p4 = std::stoi(new_line.substr(pp3 + 1));
                }
                else
                {
                    p1 = 0;
                    LOG_ERROR("Da fuck???");
                }
            }
            if (p2 == -1)
                current.collider_data = Game::Battle::ColliderData(c, static_cast<float>(p1));
            else if (p3 == -1)
                current.collider_data = Game::Battle::ColliderData(c, static_cast<float>(p1), static_cast<float>(p2));
            else
                current.collider_data = Game::Battle::ColliderData(c, static_cast<float>(p1), static_cast<float>(p2),static_cast<float>(p3),static_cast<float>(p4));
        }

        else if (line.starts_with("damage")) {
            current.damage_mul = std::stof(line.substr(7, line.size()-7+1));
        }
        else if (line.starts_with("pierce")) {
            current.pierce = std::stoi(line.substr(7, line.size()-7+1));
        }
        else if (line.starts_with("lifetime")) {
            current.lifetime = std::stoi(line.substr(9, line.size()-9+1))*1000/60;
        }
        file_free(&file_content);
    }

}

