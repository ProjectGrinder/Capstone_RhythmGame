#include "assets_manager.h"
#include "utils/print_debug.h"
#include "utils/str_utils.h"

typedef unsigned long DWORD;

extern DWORD __stdcall file_read(FileContent **content, const char *path);
extern void __stdcall file_free(FileContent **content);
extern void heap_free(void *data);
extern void *memcpy(void *dst, const void *src, size_t size);

// Leave -1 for error indication
static AssetsRecord assets_records[(SHORT_MAX - 1)] = {0};
// TODO: Change from this to proper hash mapping
static AssetsIDMapping id_map[(SHORT_MAX - 1)];
static uint16_t index = 0;


static inline assets_id make_id(uint16_t index, uint16_t gen)
{
    return ((assets_id) gen << 16) | index;
}

static inline assets_id load_assets(const char *path, const char *name, AssetsInfo info)
{
    FileContent *content = NULL;
    uint32_t id = (uint32_t) -1;
    uint16_t use_idx = (uint16_t) -1, i = 0;

    AssetsRecord *current = NULL;
    AssetsIDMapping *mapping = NULL;

    for (; i < index; ++i)
    {
        if (assets_records[i].data == NULL)
        {
            use_idx = i;
            break;
        }
    }

    if (use_idx == (uint16_t) -1)
    {
        use_idx = index;
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
    mapping = &id_map[use_idx];

    current->data = content;
    current->info = info;

    id = make_id(use_idx, current->gen);

    if (mapping->name)
        heap_free(mapping->name);

    mapping->name = strdup(name);
    mapping->id = id;

    if (use_idx == index)
        ++index;

exit:
    return (id);
}

assets_id load_sprite(const char *path, const char *name, size_t width, size_t height)
{
    AssetsInfo info = {0};
    info.type = SPRITE;
    info.info.as_sprite.height = height;
    info.info.as_sprite.width = width;
    return (load_assets(path, name, info));
}

assets_id load_vertex_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count)
{
    /* need to copy */
    InputAttributeDescription *cpy = heap_alloc(sizeof(*attributes) * count);
    memcpy(cpy, attributes, count);

    AssetsInfo info = {0};
    info.type = VERTEX_SHADER;
    info.info.as_shader.count = count;
    info.info.as_shader.data = cpy;
    return (load_assets(path, name, info));
}

assets_id load_pixel_shader(const char *path, const char *name, InputAttributeDescription *attributes, size_t count)
{
    /* need to copy */
    InputAttributeDescription *cpy = heap_alloc(sizeof(*attributes) * count);
    memcpy(cpy, attributes, count);

    AssetsInfo info = {0};
    info.type = PIXEL_SHADER;
    info.info.as_shader.count = count;
    info.info.as_shader.data = cpy;
    return (load_assets(path, name, info));
}

/* TODO: Change this shit to hash map or my ass will get whip */
assets_id get_assets_id(const char *name)
{
    size_t i = 0;
    for (; i < index; ++i)
    {
        if (strcmp(name, id_map[i].name) == 0)
            return id_map[i].id;
    }
    return (-1);
}

void free_assets(assets_id id)
{
    uint16_t index = ASSET_INDEX(id);
    AssetsRecord *current = &assets_records[index];
    AssetsIDMapping *map = &id_map[index];

    switch (current->info.type)
    {
    case VERTEX_SHADER:
    case PIXEL_SHADER:
        heap_free(current->info.info.as_shader.data);
        break;
    default:
        break;
    }

    file_free(&current->data);
    heap_free(map->name);

    ++current->gen;
    current->data = NULL;

    map->name = NULL;
    map->id = (uint32_t) -1;
}

void assets_cleanup(void)
{
    uint16_t i = 0;
    AssetsRecord *curr = NULL;
    for (; i < index; ++i)
    {
        curr = &assets_records[i];
        switch (curr->info.type)
        {
        case VERTEX_SHADER:
        case PIXEL_SHADER:
            heap_free(curr->info.info.as_shader.data);
            break;
        default:
            break;
        };
        file_free(&curr->data);
        heap_free(id_map[i].name);
    }
}
