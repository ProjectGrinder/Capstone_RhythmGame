#include <regex>
#include <sstream>
#include "game/utils/Bullethell_DSL/bullet_data_reader.h"

extern "C"{
    typedef unsigned long       DWORD;
    DWORD file_read(_Out_ FileContent **content, const char *name);
    void file_free(FileContent **file);
}

using namespace Game::Battle;
using namespace Game::BulletHell;

void CalculateCollider(const int *rect, int &p1)
{
    int width = rect[2] - rect[0];
    int height = rect[3] - rect[1];
    int size = width < height ? width : height;
    int hitbox = size / 2;
    p1 = hitbox;
}

std::string trim(const std::string& s)
{
    return std::regex_replace(s, std::regex("\\s+"), "");
}

std::vector<std::string> split(const std::string& s, const char delim)
{
    std::vector<std::string> out;

    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delim))
    {
        out.push_back(trim(item));
    }

    return out;
}


BulletRegistry Game::BulletHell::read_bullet_graphic_data_from_file(const char *filepath)
{
    BulletRegistry bullet_registry;
    FileContent *file_content;
    file_read(&file_content, filepath);

    std::string content(file_content->data, file_content->size);
    std::istringstream f(content);

    std::string line;
    int linecount = 0;

    BulletGraphicMap current;
    bool inBlock = false;

    while (std::getline(f, line))
    {
        line = trim(line);
        linecount++;
        if (line.find("BulletData{") != std::string::npos)
        {
            current = BulletGraphicMap{};
            inBlock = true;
            continue;
        }
        if (line.find('}') != std::string::npos)
        {
            bullet_registry.bulletGraphicMaps.push_back(current);
            inBlock = false;
            continue;
        }
        if (!inBlock)
            continue;

        // Necessary Info
        if (line.starts_with("rect"))
        {
            std::string new_line = line.substr(6, line.size() - 6 + 1);
            std::vector<std::string> params = split(new_line, ',');
            if (params.empty() || params.size() != 4)
                throw std::runtime_error("DSL line " + std::to_string(linecount) + " error : Rect size should be 4, but it's " + std::to_string(params.size()) + ".");

            current.graphic_data = GraphicData(
                    std::stoi(params[0]),
                    std::stoi(params[1]),
                    std::stoi(params[2]),
                    std::stoi(params[3]));

            int size;
            CalculateCollider(current.graphic_data.src_rect, size);
            current.collider_data = ColliderData(Game::Physics::CIRCLE, static_cast<float>(size));
        }

        else if (line.starts_with("color"))
        {
            auto &gd = current.graphic_data;
            std::string new_line = line.substr(7, line.size() - 7 + 1);
            std::vector<std::string> params = split(new_line, ',');
            if (params.empty() || params.size() < 3 || params.size() > 4)
                throw std::runtime_error("DSL line " + std::to_string(linecount) + " error : Color size should be between 3-4, but it's " + std::to_string(params.size()) + ".");
            gd.r = std::stof(params[0]);
            gd.g = std::stof(params[1]);
            gd.b = std::stof(params[2]);
            gd.a = (params.size() == 4)? std::stof(params[3]):1;
        }

        else if (line.starts_with("sound"))
        {
            auto &gd = current.graphic_data;
            std::string inside = line.substr(7, line.size() - 7 + 1);
            gd.bullet_spawn_sound = std::stoi(inside.substr(0));
        }

        else if (line.starts_with("collider"))
        {
            Game::Physics::ColliderType c;

            if (line.find('[') != std::string::npos)
                c = Game::Physics::RECTANGLE;

            else if (line.find('(') != std::string::npos)
                c = Game::Physics::CIRCLE;
            else
                continue;

            std::string new_line = line.substr(10, line.size() - 10 + 1);

            if (new_line.size() <= 2)
                current.collider_data.type = c;
            else
            {
                std::vector<std::string> params = split(new_line, ',');
                if (params.size()==1)
                    current.collider_data = ColliderData(c, std::stof(params[0]));
                else if (params.size() == 2)
                    current.collider_data = ColliderData(c, std::stof(params[0]), std::stof(params[1]));
                else if (params.size() == 4)
                    current.collider_data = ColliderData(c, std::stof(params[0]), std::stof(params[1]), std::stof(params[2]), std::stof(params[3]));
                else
                    throw std::runtime_error("DSL line " + std::to_string(linecount) + " error : Collider size should be 1,2 or 4, but it's " + std::to_string(params.size()) + ".");
            }
        }
        file_free(&file_content);
    }

    return bullet_registry;
}

void Game::BulletHell::read_bullet_stage_data(const std::string& content, BulletRegistry &bullet_registry)
{
    std::istringstream f(content);

    std::string line;
    int linecount = 0;

    StageBulletData current;
    bool inBlock = false;

    bullet_registry.bulletStageMaps.clear();

    while (std::getline(f, line))
    {
        line = trim(line);
        linecount++;
        if (line.find("BulletData{") != std::string::npos)
        {
            current = StageBulletData{};
            inBlock = true;
            continue;
        }
        if (line.find('}') != std::string::npos)
        {
            bullet_registry.bulletStageMaps.push_back(current);
            inBlock = false;
            continue;
        }
        if (!inBlock)
            continue;


        if (line.starts_with("bullet_type"))
        {
            std::string new_line = line.substr(12, line.size() - 12 + 1);
            std::vector<std::string> params = split(new_line, ',');
            if (params.empty() || params.size() != 3)
                throw std::runtime_error("DSL line " + std::to_string(linecount) + " error : Bullet-type size should be 3, but it's " + std::to_string(params.size()) + ".");

            if (line.find("Booming") != std::string::npos)
            {
                current.special_bullet_data = SpecialBulletData(
                        Booming,
                        std::stof(params[1]),
                        std::stoi(params[2])
                        );
                current.pierce = 999;
            }

            if (line.find("Laser") != std::string::npos)
            {
                current.special_bullet_data = SpecialBulletData(
                        Laser,
                        std::stof(params[1]),
                        std::stoi(params[2]));
                current.pierce = 999;
            }
        }

        else if (line.starts_with("size"))
        {
            std::string new_line = line.substr(6, line.size() - 6 + 1);
            std::vector<std::string> params = split(new_line, ',');
            if (params.empty() || params.size() != 2)
                throw std::runtime_error("DSL line " + std::to_string(linecount) + " error : Size size should be 2, but it's " + std::to_string(params.size()) + ".");

            current.size = Math::Point(
                    std::stof(params[0]),
                    std::stof(params[1]), 1);
        }

        else if (line.starts_with("graphic"))
        {
            current.graphicID = std::stoi(line.substr(8, line.size() - 8 + 1));
        }

        else if (line.starts_with("damage"))
        {
            current.damage_mul = std::stof(line.substr(7, line.size() - 7 + 1));
        }
        else if (line.starts_with("pierce"))
        {
            current.pierce = std::stoi(line.substr(7, line.size() - 7 + 1));
        }
        else if (line.starts_with("lifetime"))
        {
            current.bullet_timing_data.lifetime = std::stoi(line.substr(9, line.size() - 9 + 1));
        }
        else if (line.starts_with("delay"))
        {
            current.bullet_timing_data.delay_frame = std::stoi(line.substr(6, line.size() - 6 + 1));
        }
    }
}

std::vector<PatternSequence> Game::BulletHell::parse_pattern_sequences(const std::string& source)
{
    std::vector<PatternSequence> result;

    std::stringstream src(source);
    std::string line;

    while (std::getline(src, line))
    {
        line = trim(line);

        if (line.empty() || line.front() != '(' || line.back() != ')')
            continue;

        line = line.substr(1, line.size() - 2);

        std::vector<std::string> params = split(line, ',');

        if (params.empty())
            continue;

        bool loop = (params[0] == "true");

        PatternSequence seq;
        seq.isLoop = loop;

        for (size_t i = 1; i < params.size(); ++i)
        {
            seq.steps.push_back(static_cast<uint32_t>(std::stoi(params[i])));
        }

        result.push_back(seq);
    }

    return result;
}

std::vector<PatternStep> Game::BulletHell::parse_pattern_step(const std::string &source)
{
    std::vector<PatternStep> result;

    std::stringstream src(source);
    std::string line;

    while (std::getline(src, line))
    {
        line = trim(line);

        if (line.empty() || line.front() != '(' || line.back() != ')')
            continue;

        line = line.substr(1, line.size() - 2);

        std::vector<std::string> params = split(line, ',');

        if (params.size() < 3)
            continue;

        int delay = std::stoi(params[0]);
        PatternOp op;
        if (params[1]=="SET") op = OP_SET;
        else if (params[1]=="ADD") op = OP_ADD;
        else if (params[1]=="MULT") op = OP_MULT;
        else if (params[1]=="PARAM") op = OP_PARAM;
        else continue;
        auto mask = static_cast<uint8_t>(std::stoi(params[2], nullptr, 2));

        float p1 = params.size() > 3 ? std::stof(params[3]) : 0.f;
        float p2 = params.size() > 4 ? std::stof(params[4]) : 0.f;
        float p3 = params.size() > 5 ? std::stof(params[5]) : 0.f;
        float p4 = params.size() > 6 ? std::stof(params[6]) : 0.f;

        result.emplace_back(delay,op,mask,p1,p2,p3,p4);
    }

    return result;
}
