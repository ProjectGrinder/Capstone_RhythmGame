#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "system/asset_manager.h"
#include "utils/parse_glyph.h"
#include "utils/str_utils.h"

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

int parse_glyph_capstone_atlas_v1(const char *text, AssetsInfo *info)
{
    const char *cur = text;

    if (!expect_prefix(&cur, "CapstoneAtlasV1"))
    {
        return (-1);
    }
    skip_to_eol(&cur);

    if (!expect_prefix(&cur, "atlas "))
    {
        return (-1);
    }

    {
        unsigned long w = 0;
        unsigned long h = 0;

        if (!parse_uint(&cur, &w))
        {
            return (-1);
        }

        if (!parse_uint(&cur, &h))
        {
            return (-1);
        }

        info->type = FONT;
        info->info.as_font.atlas_width = (size_t) w;
        info->info.as_font.atlas_height = (size_t) h;
    }

    if (!expect_prefix(&cur, "glyphs "))
    {
        return (-1);
    }

    unsigned long count = 0;
    if (!parse_uint(&cur, &count))
    {
        return (-1);
    }
    info->info.as_font.count = (size_t) count;
    info->info.as_font.data = (GlyphAttributeDescription *) heap_alloc(sizeof(AssetsInfo) * count);

    skip_to_eol(&cur);

    if (!expect_prefix(&cur, "char u0 v0 u1 v1 width height bearingX bearingY advance"))
    {
        return (-1);
    }

    skip_to_eol(&cur);

    while (*cur && count < info->info.as_font.count)
    {
        GlyphAttributeDescription g = info->info.as_font.data[count];
        count++;
        unsigned long tmp = 0;

        if (!parse_uint(&cur, &tmp))
        {
            return (-1);
        }
        g.character = (char) tmp;
        if (!parse_float(&cur, &g.u0))
        {
            return (-1);
        }
        if (!parse_float(&cur, &g.v0))
        {
            return (-1);
        }
        if (!parse_float(&cur, &g.u1))
        {
            return (-1);
        }
        if (!parse_float(&cur, &g.v1))
        {
            return (-1);
        }

        if (!parse_uint(&cur, &tmp))
        {
            return (-1);
        }
        g.width = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            return (-1);
        }
        g.height = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            return (-1);
        }
        g.bearing_x = (int) tmp;
        if (!parse_uint(&cur, &tmp))
        {
            return (-1);
        }
        g.bearing_y = (int) tmp;
        if (!parse_float(&cur, &g.advance))
        {
            return (-1);
        }

        skip_to_eol(&cur);
    }

    return (0);
}

int parse_glyph(const char *attr_path, AssetsInfo *info)
{
    return parse_glyph_capstone_atlas_v1(attr_path, info);
}
