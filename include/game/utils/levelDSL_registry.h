#pragma once

// This should be an ASSET file? or registry comp? or something else?
namespace Game
{
    struct LevelDSL
    {
        std::string bullet_script;
    };

    inline std::vector<std::vector<LevelDSL>> levelDSL_lists = {
        // Level 1
        { {"dsl/01-L.th0"} , {"dsl/01-S.th0"}, {"dsl/01-B.th0"}},
        // Level 2
        { {"dsl/02-L.th0"} , {"dsl/02-L.th0"}, {"dsl/02-L.th0"}}
    };
} // namespace Game
