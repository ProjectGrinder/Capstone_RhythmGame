#pragma once

#include "game/components/battle/bullet_data.h"
#include "game/components/battle/pattern_container.h"

namespace Game::BulletHell
{
    Battle::BulletRegistry read_bullet_graphic_data_from_file(const char *filepath);

    void read_bullet_stage_data(const std::string& content, Battle::BulletRegistry &bullet_registry);

    std::vector<Battle::PatternSequence> parse_pattern_sequences(const std::string& source);

    std::vector<Battle::PatternStep> parse_pattern_step(const std::string& source);
}
