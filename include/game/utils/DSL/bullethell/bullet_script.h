#pragma once
#include "game/components/battle/bullet_data.h"
#include "game/components/battle/pattern_container.h"
#include "interpreter.h"

namespace Game::BulletHell
{
    class BulletScript
    {
        public:
        Battle::BulletRegistry bullet_registry;
        Battle::PatternContainer pattern_container;
        Battle::BulletLoader bullet_loader;
        explicit BulletScript(const char *graphic_path)
        {
            init_bullet_graphic(graphic_path);
        }
        explicit BulletScript(const char *graphic_path, const char *filepath)
        {
            init_bullet_graphic(graphic_path);
            read_dsl_from_file(filepath);
        }
        void init_bullet_graphic(const char *filepath);
        void read_dsl_from_file(const char *filepath);
    private:
        const char *graphic_path = nullptr;
        const char *filepath = nullptr;
        DSL::BulletHellCompiler complier;
    };

}
