#pragma once
#include <cstdint>
namespace ECS
{
    using entity_id = uint32_t;
    class EntityGenerator
    {
    private:
        entity_id _next_entity_id = 0;
    public:
        entity_id generate_entity()
        {
            return (_next_entity_id++);
        }
        void reset()
        {
            _next_entity_id = 0;
        }
    };
}