#pragma once

// FIXME: May separate thread
namespace Game::Physics
{
    struct Scale
    {
        float scaleX,scaleY,scaleZ;
        Scale() : scaleX(1),scaleY(1),scaleZ(1)
        {}
        explicit Scale(const float scale) : scaleX(scale),scaleY(scale),scaleZ(1)
        {}
        explicit Scale(const float scaleX,const float scaleY) : scaleX(scaleX),scaleY(scaleY),scaleZ(1)
        {}
        explicit Scale(const float scaleX,const float scaleY,const float scaleZ) : scaleX(scaleX),scaleY(scaleY),scaleZ(scaleZ)
        {}
    };
}