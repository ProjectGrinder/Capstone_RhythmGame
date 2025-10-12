#pragma once
#include <cstdint>

namespace System
{
    struct Config
    {
        constexpr static uint32_t VertexQueueSize = 4;
        constexpr static uint32_t RendererQueueSize = 4;
    };
}