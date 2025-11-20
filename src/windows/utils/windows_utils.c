#include "windows_utils.h"

#include <stdarg.h>
#include <strsafe.h>

static CRITICAL_SECTION log_lock;
static char log_initialize = 0;

void __vectorcall log_init()
{
    if (!log_initialize)
    {
        InitializeCriticalSection(&log_lock);
        log_initialize = 1;
    }
}

void __vectorcall log_cleanup()
{
    if (log_initialize)
    {
        DeleteCriticalSection(&log_lock);
        log_initialize = 0;
    }
}

void __cdecl log_message(_In_ LogLevel level, _In_ const char *function_name, _In_ const char *format, _In_...)
{
    /* We can use va_* because it's compiler directive hence no CRT */
    SYSTEMTIME st = {0};
    va_list args = NULL;
    HRESULT error = ERROR_SUCCESS;
    char time_buffer[64] = {0};
    char str[LOG_MAX] = {0};
    char final[LOG_MAX + 128 + 8] = {0};
    char *type = NULL;

    if (!log_initialize)
    {
        goto exit;
    }

    switch (level)
    {
    case LL_INFO:
        type = "Info";
        break;
    case LL_WARNING:
        type = "Warning";
        break;
    case LL_ERROR:
        type = "Error";
        break;
    }

    GetLocalTime(&st);
    error = StringCchPrintf(
            time_buffer,
            sizeof(time_buffer),
            "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);
    if (FAILED(error))
    {
        goto exit;
    }

    va_start(args, format);
    error = StringCchVPrintfEx(str, sizeof(str), NULL, NULL, STRSAFE_FILL_BEHIND_NULL, format, args);
    va_end(args);
    if (FAILED(error))
    {
        goto exit;
    }

    error = StringCchPrintfA(final, sizeof(final), "[ %s | %s | %s ] %s\n", type, function_name, time_buffer, str);
    if (FAILED(error))
    {
        goto exit;
    }

    EnterCriticalSection(&log_lock);
    OutputDebugString(final);
    LeaveCriticalSection(&log_lock);

exit:
    return;
}
