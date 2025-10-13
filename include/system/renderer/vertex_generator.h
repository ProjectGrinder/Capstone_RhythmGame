#pragma once
#include "system.h"

namespace System::Renderer
{
    class VertexGenerator
    {
        VertexGeneratorQueue<Config::VertexQueueSize> *_vertex_queue = nullptr;
        RendererQueue<Config::RendererQueueSize> *_renderer_queue = nullptr;

    public:
        explicit VertexGenerator(
                VertexGeneratorQueue<Config::VertexQueueSize> *vertex_queue,
                RendererQueue<Config::RendererQueueSize> *renderer_queue) :
            _vertex_queue(vertex_queue), _renderer_queue(renderer_queue) {};
    };
} // namespace System::Renderer
