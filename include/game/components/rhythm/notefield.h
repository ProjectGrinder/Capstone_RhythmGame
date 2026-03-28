#pragma once

namespace Game::Rhythm
{
    struct NoteField
    {
        float spawn_level;
        float judge_level;
        float lane1_spawn;
        float lane2_spawn;
        float lane3_spawn;
        float lane4_spawn;
        explicit NoteField(
            const float spawn_level,
            const float judge_level,
            const float lane1_spawn,
            const float lane2_spawn,
            const float lane3_spawn,
            const float lane4_spawn) :
        spawn_level(spawn_level),
        judge_level(judge_level),
        lane1_spawn(lane1_spawn),
        lane2_spawn(lane2_spawn),
        lane3_spawn(lane3_spawn),
        lane4_spawn(lane4_spawn)
        {}
    };
} // namespace Game::Rhythm
