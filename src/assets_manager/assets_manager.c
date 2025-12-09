#include "assets_manager.h"

typedef unsigned long DWORD;

extern DWORD __stdcall file_read(FileContent **content, const char *path);
extern void __stdcall file_free(FileContent **content);

// Leave -1 for error indication
static AssetsRecord assets_records[(SHORT_MAX - 1)] = {0};
// TODO: Change from this to proper hash mapping
static AssetsIDMapping id_map[(SHORT_MAX - 1)];
static uint16_t index = 0;

assets_id load_assets(const char *path, const char *name, AssetsType type)
{
    FileContent *content = NULL;
    AssetsRecord current = assets_records[index];
    AssetsIDMapping mapping = {0};
    uint32_t id = -1;

    if (file_read(&content, path) != 0)
        goto exit;

    current.data = content;
    current.type = type;
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
