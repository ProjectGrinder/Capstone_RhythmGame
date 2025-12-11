#pragma once

#ifndef NULL
    #define NULL ((void *) 0)
#endif

extern void *heap_alloc(size_t size);

static inline int strcmp(const char *left, const char *right)
{
    for (; *left && *left == *right; ++left, ++right)
        ;
    return (unsigned char) *left - (unsigned char) *right;
}

static inline size_t strlen(const char *s)
{
    size_t cnt = 0;
    while (s[cnt] != '\0')
        ++cnt;
    return (cnt);
}

static inline char *strdup(const char *s)
{
    if (!s)
        return (char *) NULL;
    size_t len = strlen(s) + 1;
    char *dst = (char *) heap_alloc(len);
    if (!dst)
        return (char *) NULL;

    for (size_t i = 0; i <= len; ++i)
        dst[i] = s[i];

    return (dst);
}
