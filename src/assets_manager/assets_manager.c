#include "assets_manager.h"
#include "utils/str_utils.h"
#include "utils/macros.h"

typedef unsigned long DWORD;

extern DWORD __stdcall file_read(FileContent **content, const char *path);
extern void __stdcall file_free(FileContent **content);
extern void heap_free(void *data);

// Leave -1 for error indication
static AssetsRecord assets_records[(SHORT_MAX - 1)] = {0};
// TODO: Change from this to proper hash mapping
static AssetsIDMapping id_map[(SHORT_MAX - 1)];
static uint16_t index = 0;

static inline assets_id load_assets(const char *path, const char *name, AssetsInfo info)
{
    FileContent *content = NULL;
    AssetsRecord current = assets_records[index];
    AssetsIDMapping mapping = {0};
    uint32_t id = -1;

    if (file_read(&content, path) != 0)
        goto exit;

    current.data = content;
    current.info = info;
    ++current.gen;

    id = make_id(index, current.gen);
    mapping.name = strdup(name);
    mapping.id = id;

    assets_records[index] = current;
    id_map[index] = mapping;

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
    InputAttributeDescription *copy = heap_alloc(sizeof(*attributes) * count);
    UNUSED(path);
    UNUSED(copy);
    UNUSED(name);
    UNUSED(attributes);
    return (0);
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
    AssetsRecord current = assets_records[index];
    AssetsIDMapping map = id_map[index];

    file_free(&current.data);
    heap_free(map.name);
}

void assets_cleanup(void)
{
    uint16_t i = 0;
    for (; i < index; ++i)
    {
        file_free(&assets_records[index].data);
        heap_free((void **) &id_map[index].name);
    }
}
