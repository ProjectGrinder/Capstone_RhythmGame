#pragma once
#pragma warning(disable : 4200)

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef SHORT_MAX
    #define SHORT_MAX 65535
#endif

#define ASSET_INDEX(id) ((uint16_t) ((id) & 0xFFFFu))
#define ASSET_GEN(id) ((uint16_t) (((id) >> 16) & 0xFFFFu))
#include "utils/input_attribute_description.h"

    typedef unsigned int uint32_t;
    typedef unsigned short uint16_t;
    typedef unsigned char byte;

    typedef struct
    {
        size_t size;
        size_t alloc;
        char data[];
    } FileContent;

    typedef uint32_t assets_id;

    typedef enum
    {
        VERTEX_SHADER,
        PIXEL_SHADER,
        SPRITE,
        FONT,
        AUDIO
    } AssetsType;

    typedef struct
    {
        char character;
        float u0;
        float v0;
        float u1;
        float v1;
        int width;
        int height;
        int bearing_x;
        int bearing_y;
        float advance;
    } GlyphAttributeDescription;

    typedef struct
    {
        char *name;
        AssetsType type;
        union
        {
            struct
            {
                size_t count;
                InputAttributeDescription *data;
            } as_shader;
            struct
            {
                size_t width, height;
            } as_sprite;
            struct
            {
                size_t count;
                GlyphAttributeDescription *data;
                size_t atlas_width, atlas_height;
            } as_font;
        } info;
    } AssetsInfo;

    typedef struct
    {
        assets_id id;
        AssetsInfo info;
        void *gpu_extension;
        FileContent *data;
    } AssetsRecord;

    typedef struct
    {
        char *name;
        assets_id id;
    } AssetsIDMapping;

    assets_id get_assets_id(const char *name);
    const AssetsRecord *load_sprite(const char *path, const char *name, size_t width, size_t height);
    const AssetsRecord *
    load_vertex_shader(const char *path, const char *name, const InputAttributeDescription *attributes, size_t count);
    const AssetsRecord *
    load_pixel_shader(const char *path, const char *name, const InputAttributeDescription *attributes, size_t count);
    const AssetsRecord *load_audio(const char *path, const char *name);
    const AssetsRecord *load_font(const char *atlas_path, const char *name, const char *attr_path);

    int has_assets(const char *name);
    AssetsRecord get_assets_record(assets_id id);
    const AssetsRecord *get_assets_record_ptr(const assets_id id);
    void free_assets(assets_id id);

    char wait_for_assets_load(void);

    void assets_cleanup(void);

#ifdef __cplusplus
}
#endif
