#include "assets_manager.hpp"

typedef unsigned char byte;
typedef size_t asset_id;

typedef struct
{

    char *semantic;
    enum
    {
        FLOAT32BITS,
        FLOAT16BITS,
        FLOAT8BITS,

        UINT32BITS,
        UINT16BITS,
        UINT8BITS,
    } type;

    size_t offset;

} InputAttributeDescription;

typedef struct
{
    InputAttributeDescription *elements;
    size_t count;
} InputLayout;

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
            InputLayout layout;
        } as_vertex_shader;

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
