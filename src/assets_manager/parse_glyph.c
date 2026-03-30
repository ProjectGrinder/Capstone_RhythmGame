#include <Windows.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "system/asset_manager.h"
#include "utils/parse_glyph.h"

#include "utils/print_debug.h"
#include "utils/str_utils.h"

extern DWORD __stdcall file_read(FileContent **content, const char *path);

static void skip_spaces(const char **cur)
{
    while (**cur == ' ' || **cur == '\t' || **cur == '\r')
        ++(*cur);
}

static void skip_to_eol(const char **cur)
{
    while (**cur && **cur != '\n')
        ++(*cur);
    if (**cur == '\n')
        ++(*cur);
}

static int expect_prefix(const char **cur, const char *prefix)
{
    size_t len = strlen(prefix);

    if (strncmp(*cur, prefix, len) != 0)
        return 0;

    *cur += len;
    return 1;
}

static int parse_uint(const char **cur, unsigned long *out)
{
    char *end = NULL;

    skip_spaces(cur);
    if (**cur < '0' || **cur > '9')
        return 0;

    *out = strtoul(*cur, &end, 10);
    if (end == *cur)
        return 0;

    *cur = end;
    return 1;
}

static int parse_float(const char **cur, float *out)
{
    char *end = NULL;

    skip_spaces(cur);
    *out = (float) strtof(*cur, &end);
    if (end == *cur)
        return 0;

    *cur = end;
    return 1;
}

int parse_glyph_capstone_atlas_v1(const char *attr_path, AssetsInfo *info)
{

    FileContent *content = NULL;

    if (file_read(&content, attr_path) != 0)
    {
        LOG_ERROR("Cannot read file at path of %s", attr_path);
        return (-1);
    }

    const char *cur = content->data;

    if (!expect_prefix(&cur, "CapstoneAtlasV1"))
    {
        LOG_ERROR("Missing CapstoneAtlasV1 signature");
        return (-1);
    }
    skip_to_eol(&cur);

    if (!expect_prefix(&cur, "atlas"))
    {
        LOG_ERROR("Missing atlas signature");
        return (-1);
    }

    {
        unsigned long w = 0;
        unsigned long h = 0;

        if (!parse_uint(&cur, &w))
        {
            LOG_ERROR("Unable to parse atlas width");
            return (-1);
        }

        if (!parse_uint(&cur, &h))
        {
            LOG_ERROR("Unable to parse atlas height");
            return (-1);
        }

        info->type = FONT;
        info->info.as_font.atlas_width = (size_t) w;
        info->info.as_font.atlas_height = (size_t) h;
    }

    skip_to_eol(&cur);

    if (!expect_prefix(&cur, "glyphs"))
    {
        LOG_ERROR("Missing glyphs signature");
        return (-1);
    }

    unsigned long count = 0;
    if (!parse_uint(&cur, &count))
    {
        LOG_ERROR("Unable to parse glyph count");
        return (-1);
    }
    info->info.as_font.count = (size_t) count;
    info->info.as_font.data = (GlyphAttributeDescription *) heap_alloc(sizeof(AssetsInfo) * count);

    skip_to_eol(&cur);

    if (!expect_prefix(&cur, "char u0 v0 u1 v1 width height bearingX bearingY advance"))
    {
        LOG_ERROR("Missing glyph attribute description");
        return (-1);
    }

    skip_to_eol(&cur);

    count = 0;

    while (*cur && count < info->info.as_font.count)
    {
        GlyphAttributeDescription g;
        count++;
        unsigned long tmp = 0;

        if (!parse_uint(&cur, &tmp))
        {
            LOG_ERROR("Unable to parse glyph index");
            return (-1);
        }
        g.character = (char) tmp;
        if (!parse_float(&cur, &g.u0))
        {
            LOG_ERROR("Unable to parse glyph u0");
            return (-1);
        }
        if (!parse_float(&cur, &g.v0))
        {
            LOG_ERROR("Unable to parse glyph v0");
            return (-1);
        }
        if (!parse_float(&cur, &g.u1))
        {
            LOG_ERROR("Unable to parse glyph u1");
            return (-1);
        }
        if (!parse_float(&cur, &g.v1))
        {
            LOG_ERROR("Unable to parse glyph v1");
            return (-1);
        }

        if (!parse_uint(&cur, &tmp))
        {
            LOG_ERROR("Unable to parse glyph width");
            return (-1);
        }
        g.width = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            LOG_ERROR("Unable to parse glyph height");
            return (-1);
        }
        g.height = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            LOG_ERROR("Unable to parse glyph bearingX");
            return (-1);
        }
        g.bearing_x = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            LOG_ERROR("Unable to parse glyph bearingY");
            return (-1);
        }
        g.bearing_y = (int) tmp;
        if (!parse_float(&cur, &g.advance))
        {
            LOG_ERROR("Unable to parse glyph advance");
            return (-1);
        }

        info->info.as_font.data[count] = g;

        // LOG_INFO(
        //         "%c %d %d %d %d",
        //         info->info.as_font.data[count].character,
        //         info->info.as_font.data[count].width,
        //         info->info.as_font.data[count].height,
        //         info->info.as_font.data[count].bearing_x,
        //         info->info.as_font.data[count].bearing_y)

        skip_to_eol(&cur);
    }

    return (0);
}

int parse_glyph(const char *attr_path, AssetsInfo *info)
{
    return parse_glyph_capstone_atlas_v1(attr_path, info);
}
