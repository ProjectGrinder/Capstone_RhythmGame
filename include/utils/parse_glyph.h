#pragma once
#include "system/asset_manager.h"

#ifdef __cplusplus
extern "C"
{
#endif

    int parse_glyph(const char *attr_path, AssetsInfo *info);
    int parse_glyph_capstone_atlas_v1(const char *attr_path, AssetsInfo *info);

#ifdef __cplusplus
}
#endif