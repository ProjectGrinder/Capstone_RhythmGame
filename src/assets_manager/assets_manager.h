#pragma once

#ifndef SHORT_MAX
    #define SHORT_MAX 65534
#endif

#ifndef NULL
    #define NULL ((void *) 0)
#endif

#define ASSET_INDEX(id) ((uint16_t) ((id) & 0xFFFFu))
#define ASSET_GEN(id) ((uint16_t) (((id) >> 16) & 0xFFFFu))

extern void *heap_alloc(size_t size);

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char byte;

typedef uint32_t assets_id;

typedef struct
{
    size_t size;
    size_t alloc;
    char data[];
} FileContent;

typedef enum
{
    VERTEX_SHADER,
    PIXEL_SHADER,
    SPRITE,
} AssetsType;

typedef struct
{
    AssetsType type;
    uint16_t gen;
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

static inline int strcmp(const char *left, const char *right)
{
    for (; *left && *left == *right; ++left, ++right)
        ;
    return (unsigned char) *left - (unsigned char) *right;
}

static inline size_t strlen(const char *s)
{
    size_t cnt = 0;
    while (s[cnt] != '\0')
        ++cnt;
    return (cnt);
}

static inline char *strdup(const char *s)
{
    if (!s)
        return (char *) NULL;
    size_t len = strlen(s) + 1;
    char *dst = (char *) heap_alloc(len);
    if (!dst)
        return (char *) NULL;

    for (size_t i = 0; i <= len; ++i)
        dst[i] = s[i];

    return (dst);
}

assets_id load_assets(const char *path, const char *name, AssetsType type);

assets_id get_assets_id(const char *name);

char wait_for_assets_load();
