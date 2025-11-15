#pragma once
#include <Windows.h>

#ifndef PROJECT_NAME
    #define PROJECT_NAME    UNNAMED_PROJECT
#endif

#define STR(name)       #name
#define TO_STR(name)    STR(name)

#define LOG_MAX         512

#define LOG_INFO(format, ...)       log_message(LL_INFO, __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_WARNING(format, ...)    log_message(LL_WARNING, __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...)      log_message(LL_ERROR, __FUNCTION__, format, ##__VA_ARGS__);

typedef enum
{
    LL_INFO,
    LL_WARNING,
    LL_ERROR,
} LogLevel;

void __vectorcall log_init();
void __vectorcall log_cleanup();
void __cdecl log_message
(
    _In_    LogLevel    level,
    _In_    const char* function_name,
    _In_    const char* format,
    _In_    ...
);

static __inline size_t __vectorcall char_string_get_size(_In_ const char *str)
{
    size_t cnt = 0;
    while (str[cnt] != '\0')
        ++cnt;
    return (cnt);
}