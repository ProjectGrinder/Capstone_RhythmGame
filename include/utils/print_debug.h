#pragma once

#ifndef PROJECT_NAME
    #define PROJECT_NAME UNNAMED_PROJECT
#endif

#define STR(name) #name
#define TO_STR(name) STR(name)

#define LOG_MAX 512

#define LOG_INFO(format, ...) log_message(LogLevel::LL_INFO, __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_WARNING(format, ...) log_message(LogLevel::LL_WARNING, __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...) log_message(LogLevel::LL_ERROR, __FUNCTION__, format, ##__VA_ARGS__);

typedef enum class LogLevel : char
{
    LL_INFO,
    LL_WARNING,
    LL_ERROR,
} LogLevel;

#ifdef __cplusplus
extern "C"
{
#endif

    void __cdecl log_message(LogLevel level, const char *function_name, const char *format, ...);

#ifdef __cplusplus
}
#endif
