#pragma once

namespace ECS
{
    using Entity = uint32_t;
    class EntityGenerator
    {
    private:
        static EntityGenerator _instance;
        Entity _next_entity_id = 0;
        EntityGenerator() = default;
    public:
        static EntityGenerator& instance()
        {
            return (_instance);
        }
        Entity generate_entity()
        {
            return (_next_entity_id++);
        }
        void reset()
        {
            _next_entity_id = 0;
        }
    };
}