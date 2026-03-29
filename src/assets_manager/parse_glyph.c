#include "utils/parse_glyph.h"
#include <windows.h>

#include "utils/print_debug.h"

extern DWORD __stdcall file_read(FileContent **content, const char *path);
extern void __stdcall file_free(FileContent **content);
extern void *heap_alloc(size_t size);
extern void heap_free(void *data);
extern void *memcpy(void *dst, const void *src, size_t size);

int parse_glyph(const char *attr_path, AssetsInfo *info)
{
    return parse_glyph_capstone_atlas_v1(attr_path, info);
}

int parse_glyph_capstone_atlas_v1(const char *attr_path, AssetsInfo *info)
{
    int result = 0;
    FileContent *content = NULL;
    if (file_read(&content, attr_path) != 0)
    {
        LOG_ERROR("Cannot read file at path of %s", attr_path);
        result = -1;
        goto exit;
    }

    

    // TODO: implement
    (void) info;

exit:
    file_free(&content);
    return (result);
}