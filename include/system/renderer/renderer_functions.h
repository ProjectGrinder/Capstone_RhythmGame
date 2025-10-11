#pragma once
#include "renderer/renderer_types.h"

namespace System::Renderer
{
  template<RendererConcept Renderer>
  void render(Renderer renderer, std::priority_queue<RenderItem> queue)
  {
    for (RenderItem &item: queue)
    {
      renderer.render(item);
    }
  }
}