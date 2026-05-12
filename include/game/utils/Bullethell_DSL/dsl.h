#pragma once
#include <regex>
#include <sstream>


#include "bullet_data_reader.h"
#include "interpreter.h"
#include "parser.h"
#include "tokenizer.h"

namespace Game::BulletHell
{
    inline std::unordered_map<std::string, std::string> dsl_sections;

    inline bool split_section(const std::string &src)
    {
        std::stringstream ss(src);
        std::string line;

        std::string current_section;

        while (std::getline(ss, line))
        {
            line = std::regex_replace(line, std::regex("\\s+"), "");

            if (!line.empty() && line[0] == '#')
            {
                current_section = line.substr(1);
                dsl_sections[current_section];
                continue;
            }

            if (current_section.empty())
                continue;

            dsl_sections[current_section] += line + '\n';
        }

        return true;
    }

    inline void read_dsl_from_file(const char *filepath, Battle::BulletRegistry &bullet_registry)
    {
        FileContent* file_content;
        file_read(&file_content, filepath);

        const std::string src(file_content->data, file_content->size);

        split_section(src);

        Battle::PatternContainer pattern_container{
            parse_pattern_step(dsl_sections["Pattern"]),
            parse_pattern_sequences(dsl_sections["PatternSeq"])
        };

        read_bullet_stage_data(dsl_sections["BulletData"],bullet_registry);
    }
}
