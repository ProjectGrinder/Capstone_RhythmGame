#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
    typedef enum
    {
        R32G32B32A32_FLOAT,
        R32G32B32_FLOAT,
        R32G32_FLOAT,

        FLOAT32BITS,
        FLOAT16BITS,

        UINT32BITS,
        UINT16BITS,
        UINT8BITS,
    } InputType;

    typedef struct
    {
        const char *semantic;
        InputType type;
        size_t offset;
    } InputAttributeDescription;

#ifdef __cplusplus
}
#endif

