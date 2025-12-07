#include "assets_manager.hpp"

typedef unsigned char byte;
typedef size_t asset_id;

typedef struct
{

    enum
    {
        ASSET_PIXEL_SHADER,
        ASSET_VERTEX_SHADER,
        ASSET_IMAGE,
        ASSET_UNKNOWN,
    } type;
    size_t size;
    union
    {
        struct
        {
            size_t width, height;
        } as_image;

        struct
        {
            char *semantic;
            unsigned int semantic_index;
        } *as_vertex_shader;

        byte *as_any;
    } info;
    byte data[];

} AssetsData;

typedef struct
{

    char **loaded;
    AssetsData *data;
    size_t size;
    size_t alloc;

} AssetsManager;
