#pragma once
#include <fstream>
#include "system/asset_manager.h"
#include "utils/print_debug.h"

extern "C" int parse_glyph(const char *attr_path, AssetsInfo *info);

inline int parse_glyph_capstone_atlas_v1(const char *attr_path, AssetsInfo *info)
{
    constexpr int MAX_GLYPHS = 1024;

    if (info == nullptr)
    {
        LOG_ERROR("No AssetsInfo info argument");
        return (-1);
    }

    std::string in_string;
    int in_int;
    std::ifstream file(attr_path);
    if (!file.is_open())
    {
        LOG_ERROR("Failed to open file %s", attr_path);
        return (-1);
    }

    file >> in_string;
    if (in_string != "CapstoneAtlasV1")
    {
        LOG_ERROR("File is not a CapstoneAtlasV1 attribute file");
        return (-1);
    }

    file >> in_string >> in_int;
    if (in_string != "atlas")
    {
        LOG_ERROR("Invalid format: expected 'atlas width height' on line 2");
        return (-1);
    }

    info->info.as_font.atlas_width = static_cast<uint32_t>(in_int);

    file >> in_int;
    info->info.as_font.atlas_height = static_cast<uint32_t>(in_int);

    file >> in_string >> in_int;
    if (in_string != "glyphs")
    {
        LOG_ERROR("Invalid format: expected 'glyphs count' on line 3");
        return (-1);
    }

    if (in_int > MAX_GLYPHS)
    {
        LOG_ERROR("Too many glyphs in this atlas");
        return (-1);
    }

    info->info.as_font.count = static_cast<uint32_t>(in_int);
    info->info.as_font.data =
            (GlyphAttributeDescription *) (malloc(info->info.as_font.count * sizeof(GlyphAttributeDescription)));

    if (info->info.as_font.data == nullptr)
    {
        LOG_ERROR("Failed to allocate GlyphAttributeDescription array");
        return (-1);
    }

    file >> in_string;
    if (in_string != "char")
    {
        LOG_ERROR("Invalid format: expected 'char' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "u0")
    {
        LOG_ERROR("Invalid format: expected 'u0' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "v0")
    {
        LOG_ERROR("Invalid format: expected 'v0' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "u1")
    {
        LOG_ERROR("Invalid format: expected 'u1' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "v1")
    {
        LOG_ERROR("Invalid format: expected 'v1' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "width")
    {
        LOG_ERROR("Invalid format: expected 'width' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "height")
    {
        LOG_ERROR("Invalid format: expected 'height' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "bearingX")
    {
        LOG_ERROR("Invalid format: expected 'bearingX' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "bearingY")
    {
        LOG_ERROR("Invalid format: expected 'bearingY' on line 4");
        return (-1);
    }

    file >> in_string;
    if (in_string != "advance")
    {
        LOG_ERROR("Invalid format: expected 'advance' on line 4");
        return (-1);
    }

    for (int i = 0; i < info->info.as_font.count; i++)
    {
        file >> in_int;
        info->info.as_font.data[i].character = static_cast<char>(in_int);

        file >> info->info.as_font.data[i].u0;
        file >> info->info.as_font.data[i].v0;
        file >> info->info.as_font.data[i].u1;
        file >> info->info.as_font.data[i].v1;
        file >> info->info.as_font.data[i].width;
        file >> info->info.as_font.data[i].height;
        file >> info->info.as_font.data[i].bearing_x;
        file >> info->info.as_font.data[i].bearing_y;
        file >> info->info.as_font.data[i].advance;
    }

    return (0);
}
