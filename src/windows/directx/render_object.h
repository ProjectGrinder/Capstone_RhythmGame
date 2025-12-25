#pragma once

typedef unsigned char byte;
typedef size_t asset_id;

typedef enum
{
    FLOAT32BITS,
    FLOAT16BITS,
    FLOAT8BITS,

    UINT32BITS,
    UINT16BITS,
    UINT8BITS,
} InputType;

typedef struct
{
    char *semantic;
    InputType type;
    size_t offset;
} InputAttributeDescription;

typedef struct
{
    size_t count;
    size_t stride;
    InputAttributeDescription *elements;
} InputLayout;

typedef enum
{
    ASSET_PIXEL_SHADER,
    ASSET_VERTEX_SHADER,
    ASSET_IMAGE,
    ASSET_UNKNOWN,
} AssetType;

typedef struct
{
    AssetType type;
    size_t size;
    size_t alloc;
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
    long long x, y, z;
} Position;

typedef struct
{
    size_t count;
    byte position[];
} VertexData;

typedef struct
{
    AssetsData *assets;
    VertexData *vertex;
} RenderObject;
