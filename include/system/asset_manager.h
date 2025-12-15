#pragma once

#include <cstdint>
extern "C"
{
    typedef uint32_t assets_id;
    typedef enum
    {
        VERTEX_SHADER,
        PIXEL_SHADER,
        SPRITE,
    } AssetsType;

    assets_id get_assets_id(const char *name);

    char wait_for_assets_load(void);
}
