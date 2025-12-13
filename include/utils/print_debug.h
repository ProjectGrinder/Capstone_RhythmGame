#pragma once

#ifndef PROJECT_NAME
    #define PROJECT_NAME UNNAMED_PROJECT
#endif

#define STR(name) #name
#define TO_STR(name) STR(name)

#define LOG_MAX 512

#ifdef __cplusplus
    #define LOG_SIG(level) LogLevel::level
#else
    #define LOG_SIG(level) level
#endif

#define LOG_INFO(format, ...) log_message(LOG_SIG(LL_INFO), __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_WARNING(format, ...) log_message(LOG_SIG(LL_WARNING), __FUNCTION__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...) log_message(LOG_SIG(LL_ERROR), __FUNCTION__, format, ##__VA_ARGS__);

#ifdef __cplusplus
typedef enum class LogLevel : char
{
    LL_INFO,
    LL_WARNING,
    LL_ERROR,
} LogLevel;
#else
typedef enum
{
    LL_INFO,
    LL_WARNING,
    LL_ERROR,
} LogLevel;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    void __cdecl log_message(LogLevel level, const char *function_name, const char *format, ...);

#ifdef __cplusplus
}
#endif
