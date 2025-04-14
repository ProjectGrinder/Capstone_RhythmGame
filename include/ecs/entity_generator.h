#pragma once
#include <cstdint>
namespace ECS
{
    using Entity = uint32_t;
    class EntityGenerator
    {
    private:
        Entity _next_entity_id = 0;
    public:
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