#pragma once

//
namespace Game::Render
{
    // Should this be implemented somewhere else
    struct Rect
    {
        float u0,v0,u1,v1;
        Rect() : u0(0),v0(0),u1(0),v1(0) {}
        Rect(const float u0, const float v0, const float u1, const float v1) : u0(u0),v0(v0),u1(u1),v1(v1)
        {}
    };

    struct Sprite
    {
        char *texture; // Texture name to uniform in
        Rect src_rect; // UV from sprite to sample from.
        Rect dst_rect; // UV applied on scene (Usually center {-halfX,-halfY,halfX,halfY})
        uint32_t layer; // Layer to render
        uint32_t order; // Order of render in layer
        bool flipX = false; // Flip Image in X-axis
        bool flipY = false; // Flip Image in Y-axis

        Sprite() : texture(nullptr), layer(0), order(0)
        {}

        explicit Sprite(char *texture, const Rect src_rect) :
            texture(texture), src_rect(src_rect), layer(0), order(0)
        {
            const float w = src_rect.u1 - src_rect.u0;
            const float h = src_rect.v1 - src_rect.v0;
            dst_rect = {-w/2,-h/2,w/2,h/2};
        }

        explicit Sprite(char *texture, const Rect src_rect, const Rect dst_rect) :
            texture(texture), src_rect(src_rect), dst_rect(dst_rect), layer(0), order(0)
        {}
    };
}