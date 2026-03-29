#pragma warning(disable : 4200)
#pragma warning(disable : 4245)

#include "system/asset_manager.h"
#include "utils/print_debug.h"
#include "utils/str_utils.h"
#include "utils/parse_glyph.h"

typedef unsigned long DWORD;

extern DWORD __stdcall file_read(FileContent **content, const char *path);
extern void __stdcall file_free(FileContent **content);
extern void *heap_alloc(size_t size);
extern void heap_free(void *data);
extern void *memcpy(void *dst, const void *src, size_t size);
extern char *strdup(const char *src);
extern void vertex_shader_release(void **shader_handler);
extern void pixel_shader_release(void **shader_handler);

// Leave -1 for error indication
static AssetsRecord assets_records[(SHORT_MAX - 1)] = {0};
// TODO: Change from this to proper hash mapping
static AssetsIDMapping id_map[(SHORT_MAX - 1)];
static uint16_t record_index = 0;

static InputAttributeDescription *copy_input_attributes(const InputAttributeDescription *attributes, size_t count)
{
    size_t bytes = 0;
    InputAttributeDescription *copy = NULL;

    if (attributes == NULL || count == 0)
    {
        return NULL;
    }

    bytes = sizeof(InputAttributeDescription) * count;
    copy = (InputAttributeDescription *) heap_alloc(bytes);
    if (copy == NULL)
    {
        LOG_ERROR("Failed to allocate shader input attribute copy");
        return NULL;
    }

    memcpy(copy, attributes, bytes);
    return copy;
}

static inline assets_id make_id(uint16_t index, uint16_t gen)
{
    return ((assets_id) gen << 16) | index;
}

static inline AssetsRecord *load_assets(const char *path, const char *name, AssetsInfo info)
{

    FileContent *content = NULL;
    uint32_t id = (uint32_t) -1;
    uint16_t use_idx = (uint16_t) -1, i = 0;

    AssetsRecord *current = NULL;
    AssetsIDMapping *mapping = NULL;

    for (; i < record_index; ++i)
    {
        if (assets_records[i].data == NULL)
        {
            use_idx = i;
            break;
        }
    }

    if (use_idx == (uint16_t) -1)
    {
        use_idx = record_index;
    }

    if (use_idx >= (SHORT_MAX - 1))
    {
        LOG_ERROR("Cannot add more assets");
        goto exit;
    }

    if (file_read(&content, path) != 0)
    {
        LOG_ERROR("Cannot read file at path of %s", path);
        goto exit;
    }

    current = &assets_records[use_idx];
    current->gpu_extension = NULL;
    mapping = &id_map[use_idx];

    current->data = content;
    current->info = info;

    id = make_id(use_idx, ASSET_GEN(current->id));

    if (mapping->name)
        heap_free(mapping->name);

    mapping->name = strdup(name);
    mapping->id = id;
    if (use_idx == record_index)
        ++record_index;

exit:
    return (current);
}

const AssetsRecord *load_sprite(const char *path, const char *name, size_t width, size_t height)
{
    AssetsInfo info = {0};
    info.name = strdup(name);
    info.type = SPRITE;
    info.info.as_sprite.height = height;
    info.info.as_sprite.width = width;
    return (load_assets(path, name, info));
}

const AssetsRecord *
load_vertex_shader(const char *path, const char *name, const InputAttributeDescription *attributes, size_t count)
{
    AssetsInfo info = {0};
    info.name = strdup(name);
    info.type = VERTEX_SHADER;
    info.info.as_shader.count = count;
    info.info.as_shader.data = copy_input_attributes(attributes, count);
    if (count > 0 && info.info.as_shader.data == NULL)
    {
        return NULL;
    }
    return (load_assets(path, name, info));
}

const AssetsRecord *
load_pixel_shader(const char *path, const char *name, const InputAttributeDescription *attributes, size_t count)
{
    AssetsInfo info = {0};
    info.name = strdup(name);
    info.type = PIXEL_SHADER;
    info.info.as_shader.count = count;
    info.info.as_shader.data = copy_input_attributes(attributes, count);
    if (count > 0 && info.info.as_shader.data == NULL)
    {
        return NULL;
    }
    return (load_assets(path, name, info));
}

const AssetsRecord *load_font(const char *atlas_path, const char *name, const char *attr_path)
{
    // attr_path is filepath for glyph attribute data
    AssetsInfo info = {0};
    info.name = strdup(name);
    info.type = FONT;
    parse_glyph(attr_path, &info);
    return (load_assets(atlas_path, name, info));
}

/* TODO: Change this shit to hash map or my ass will get whip */
assets_id get_assets_id(const char *name)
{
    size_t i = 0;
    for (; i < record_index; ++i)
    {
        if (strcmp(name, id_map[i].name) == 0)
            return id_map[i].id;
    }

    return (-1);
}

// I'm so uncivilized - Midfield
int has_assets(const char *name)
{
    return (get_assets_id(name) != (uint32_t) -1);
}

AssetsRecord get_assets_record(const assets_id id)
{
    const uint16_t index = ASSET_INDEX(id);
    return (assets_records[index]);
}

const AssetsRecord *get_assets_record_ptr(const assets_id id)
{
    const uint16_t index = ASSET_INDEX(id);
    return (&assets_records[index]);
}

void free_assets(assets_id id)
{
    uint16_t index = ASSET_INDEX(id);
    AssetsRecord *current = &assets_records[index];
    AssetsIDMapping *map = &id_map[index];

    if (current->info.type == VERTEX_SHADER || current->info.type == PIXEL_SHADER)
    {
        heap_free(current->info.info.as_shader.data);
        current->info.info.as_shader.data = NULL;
        current->info.info.as_shader.count = 0;
    }

    if (current->info.type == FONT)
    {
        heap_free(current->info.info.as_font.data);
        current->info.info.as_font.data = NULL;
        current->info.info.as_font.count = 0;
        current->info.info.as_font.atlas_width = 0;
        current->info.info.as_font.atlas_height = 0;
    }

    file_free(&current->data);
    heap_free(current->info.name);
    heap_free(map->name);

    current->id = make_id(ASSET_INDEX(current->id), ASSET_GEN(current->id) + 1);
    current->data = NULL;

    map->name = NULL;
    map->id = (uint32_t) -1;
}

void assets_cleanup(void)
{
    uint16_t i = 0;
    AssetsRecord *curr = NULL;
    for (; i < record_index; ++i)
    {
        curr = &assets_records[i];
        switch (curr->info.type)
        {
        case VERTEX_SHADER:
            heap_free(curr->info.info.as_shader.data);
            curr->info.info.as_shader.data = NULL;
            curr->info.info.as_shader.count = 0;
            if (curr->gpu_extension != NULL)
            {
                vertex_shader_release(&curr->gpu_extension);
                heap_free(curr->gpu_extension);
            }
            break;
        case PIXEL_SHADER:
            heap_free(curr->info.info.as_shader.data);
            curr->info.info.as_shader.data = NULL;
            curr->info.info.as_shader.count = 0;
            if (curr->gpu_extension != NULL)
            {
                pixel_shader_release(&curr->gpu_extension);
                heap_free(curr->gpu_extension);
            }
            break;
        default:
            break;
        };
        file_free(&curr->data);
        heap_free(id_map[i].name);
    }
}
