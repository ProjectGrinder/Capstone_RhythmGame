#include "system/renderer.h"

/* Constructor */
namespace System::Renderer
{
    RenderItem::RenderItem([[maybe_unused]]Render2D &render_2d)
    {

    }

    RenderItem::RenderItem([[maybe_unused]]Render3D &render_3d)
    {

    }

    bool RenderItem::operator<(const RenderItem &other) const
    {
        return (this->priority < other.priority);
    }

    bool RenderItem::operator>(const RenderItem &other) const
    {
        return (this->priority > other.priority);
    }

}
