#pragma once

#ifndef SHORT_MAX
    #define SHORT_MAX 65535
#endif

#define ASSET_INDEX(id) ((uint16_t) ((id) & 0xFFFFu))
#define ASSET_GEN(id) ((uint16_t) (((id) >> 16) & 0xFFFFu))

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char byte;

typedef struct
{
    size_t size;
    size_t alloc;
    char data[];
} FileContent;

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

typedef uint32_t assets_id;

typedef enum
{
    VERTEX_SHADER,
    PIXEL_SHADER,
    SPRITE,
} AssetsType;

typedef struct
{
    AssetsType type;
    union
    {
        struct { size_t count; InputAttributeDescription *data; } *as_shader;
        struct { size_t width, height;} as_sprite;
    } info;
} AssetsInfo;

typedef struct
{
    uint16_t gen;
    AssetsInfo info;
    FileContent *data;
} AssetsRecord;

typedef struct
{
    char *name;
    assets_id id;
} AssetsIDMapping;

static inline assets_id make_id(uint16_t index, uint16_t gen)
{
    return ((assets_id) gen << 16) | index;
}

assets_id get_assets_id(const char *name);

void free_assets(assets_id id);

char wait_for_assets_load(void);

void assets_cleanup(void);
