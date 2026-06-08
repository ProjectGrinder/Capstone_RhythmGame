#include <regex>
#include <sstream>

#include "game/utils/Bullethell_DSL/bullet_script.h"

#include "game/utils/Bullethell_DSL/bullet_data_reader.h"
#include "system/asset_manager.h"

extern "C"{
    typedef unsigned long       DWORD;
    DWORD file_read(_Out_ FileContent **content, const char *name);
    void file_free(FileContent **file);
}

using namespace Game::BulletHell;

bool check_header(const std::string& header)
{
    if (header == "Pattern") return true;
    if (header == "PatternSeq") return true;
    if (header == "BulletData") return true;
    if (header == "Stage") return true;
    return false;
}

void trim_r(std::string &s)
{
    const char* ws = " \t\r";

    size_t b = 0;
    while (b < s.size() && strchr(ws, s[b])) ++b;

    size_t e = s.size();
    while (e > b && strchr(ws, s[e - 1])) --e;

    s.erase(e);
    s.erase(0, b);
}

std::unordered_map<std::string, std::string> split_section(const std::string &src)
{
    std::unordered_map<std::string, std::string> dsl_sections;
    std::stringstream ss(src);
    std::string line;

    std::string current_section;

    while (std::getline(ss, line))
    {
        trim_r(line);
        if (!line.empty() && line[0] == '#')
        {
            current_section = line.substr(1);
            if (!check_header(current_section))
                throw std::runtime_error("error : Wrong header name -> " + current_section);
            dsl_sections[current_section];
            continue;
        }

        if (current_section.empty())
            continue;

        dsl_sections[current_section] += line + '\n';
    }

    return dsl_sections;
}

void BulletScript::init_bullet_graphic(const char *filepath)
{
    if (graphic_path == filepath && !bullet_registry.bulletGraphicMaps.empty())
        return;

    graphic_path = filepath;
    bullet_registry = read_bullet_graphic_data_from_file(graphic_path);
}

void BulletScript::read_dsl_from_file(const char *filepath)
{
    if (this->filepath == filepath) return;
    this->filepath = filepath;

    FileContent* file_content;
    file_read(&file_content, filepath);

    const std::string src(file_content->data, file_content->size);

    std::unordered_map<std::string, std::string> dsl_sections = split_section(src);

    pattern_container = {
        parse_pattern_step(dsl_sections["Pattern"]),
        parse_pattern_sequences(dsl_sections["PatternSeq"])
    };

    read_bullet_stage_data(dsl_sections["BulletData"],bullet_registry);

    // Check for cache

    // ----

    bullet_loader = complier.extract_bullets(dsl_sections["Stage"]);

    file_free(&file_content);
}

