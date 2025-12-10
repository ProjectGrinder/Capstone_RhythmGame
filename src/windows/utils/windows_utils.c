#include "windows_utils.h"

static CRITICAL_SECTION log_lock;
static char log_initialize = 0;

void log_init(void)
{
    if (!log_initialize)
    {
        InitializeCriticalSection(&log_lock);
        log_initialize = 1;
    }
}

void log_cleanup(void)
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
    wsprintf(
            time_buffer,
            "%04d-%02d-%02d %02d:%02d:%02d.%03d",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds);

    va_start(args, format);
    wvsprintf(str, format, args);
    va_end(args);

    wsprintf(final, "[ %s | %s | %s ] %s\n", type, function_name, time_buffer, str);

    EnterCriticalSection(&log_lock);
    OutputDebugString(final);
    LeaveCriticalSection(&log_lock);

exit:
    return;
}

DWORD __stdcall file_read(_Out_ FileContent **content, const char *path)
{
    FileContent *fc = NULL;
    LARGE_INTEGER size = {0};
    DWORD error = ERROR_SUCCESS, byte_read = 0;

    const HANDLE hfile =
            CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        LOG_ERROR("CreateFile failed, Code 0x%081x", error)
        goto exit;
    }

    if (GetFileSizeEx(hfile, &size) == 0)
    {
        error = GetLastError();
        LOG_ERROR("GetFileSizeEx failed, Code 0x%081x", error);
        goto error;
    }

    if (size.QuadPart == 0)
    {
        error = ERROR_EMPTY;
        LOG_WARNING("%s is empty", path);
        goto error;
    }

    fc = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(FileContent) + size.QuadPart + 1);
    if (fc == NULL)
    {
        error = ERROR_NOT_ENOUGH_MEMORY;
        LOG_WARNING("Cannot allocate memory for File");
        goto error;
    }
    fc->size = size.QuadPart;
    fc->alloc = size.QuadPart + 1;

    if (ReadFile(hfile, fc->data, size.QuadPart, &byte_read, NULL) == 0 || byte_read != size.QuadPart)
    {
        error = GetLastError();
        LOG_ERROR("ReadFile failed, Code 0x%081x", error);
        goto error;
    }

    fc->data[size.QuadPart] = '\0';

    *content = fc;

error:
    if (CloseHandle(hfile) == 0)
    {
        error = GetLastError();
        LOG_ERROR("CloseHandle failed, Code 0x%081x", error);
    }

exit:
    return (error);
}

void __stdcall file_free(FileContent **file)
{
    if (file && *file)
    {
        HeapFree(GetProcessHeap(), 0, *file);
        *file = NULL;
    }
}

void *__stdcall heap_alloc(size_t size)
{
    return (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size));
}

void __stdcall heap_free(void **ptr)
{
    if (ptr == NULL)
        return;
    HeapFree(GetProcessHeap(), 0, *ptr);
    ptr = NULL;
}
