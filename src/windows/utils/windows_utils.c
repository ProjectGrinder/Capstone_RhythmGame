#include "windows_utils.h"
#define SMALL_SIZE 64

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

void log_message(_In_ LogLevel level, _In_ const char *function_name, _In_ const char *format, _In_...)
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

BOOL join_path(char *dst, size_t dst_size, const char *dir, size_t dir_size, const char *file, size_t file_size)
{
    if (dir_size + 1 + file_size + 1 > dst_size)
        return (FALSE);

    RtlCopyMemory(dst, dir, dir_size);

    dst[dir_size] = '\\';
    RtlCopyMemory(dst + dir_size + 1, file, file_size);
    dst[dir_size + 1 + file_size] = '\0';

    return (TRUE);
}

DWORD file_read(_Out_ FileContent **content, _In_ const char *path)
{
    FileContent *fc = NULL;
    LARGE_INTEGER size = {0};
    DWORD error = ERROR_SUCCESS, byte_read = 0, last_backslash = 0;
    char current_directory[MAX_PATH], full_path[MAX_PATH];

    byte_read = GetModuleFileName(NULL, current_directory, sizeof(current_directory));
    if (byte_read == 0 || byte_read == MAX_PATH)
    {
        error = GetLastError();
        LOG_ERROR("GetModuleFileName failed, Code 0x%081x", error);
        goto exit;
    }

    last_backslash = byte_read;
    while (last_backslash >= 0 && current_directory[last_backslash] != '\\' && current_directory[last_backslash] != '/')
        --last_backslash;

    if (!join_path(full_path, MAX_PATH, current_directory, last_backslash, path, strlen(path)))
    {
        LOG_ERROR("Path is too long");
        goto exit;
    }

    LOG_INFO("Openning file at: %s", full_path);
    byte_read = 0;
    const HANDLE hfile =
            CreateFile(full_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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

void file_free(FileContent **file)
{
    if (file && *file)
    {
        HeapFree(GetProcessHeap(), 0, *file);
        *file = NULL;
    }
}

void *heap_alloc(size_t size)
{
    return (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size));
}

void heap_free(void *ptr)
{
    if (ptr == NULL)
        return;
    HeapFree(GetProcessHeap(), 0, ptr);
    ptr = NULL;
}

extern void *__asm_memset(void *dest, int ch, size_t count);

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

static FORCEINLINE void *__inline_memset(void *dest, int ch, size_t count)
{
    unsigned char *ptr = (unsigned char *) dest;

    unsigned long long fill_ull = (unsigned long long) (unsigned char) ch * 0x0101010101010101ULL;
    unsigned int fill_ui = (unsigned int) (unsigned char) ch * 0x01010101U;
    unsigned short fill_us = (unsigned short) (unsigned char) ch * 0x0101U;

    while (count >= 8)
    {
        *(u64 *) ptr = fill_ull;
        ptr += 8;
        count -= 8;
    }

    if (count >= 4)
    {
        *(u32 *) ptr = fill_ui;
        ptr += 4;
        count -= 4;
    }

    if (count >= 2)
    {
        *(u16 *) ptr = fill_us;
        ptr += 2;
        count -= 2;
    }
    if (count)
    {
        *ptr = (unsigned char) ch;
    }

    return (dest);
}

void *memset(void *dest, int ch, size_t count)
{
    if (count >= SMALL_SIZE)
        return (__asm_memset(dest, ch, count));
    return (__inline_memset(dest, ch, count));
}

extern void *__asm_memcpy(void *dest, const void *src, size_t size);

static FORCEINLINE void *__inline_memcpy(void *dest, const void *src, size_t size)
{
    u8 *dptr = (u8 *) dest;
    const u8 *sptr = (const u8 *) src;

    if (((size_t) dptr | (size_t) sptr) % 8 == 0)
    {
        while (size >= 8)
        {
            *(u64 *) dptr = *(u64 *) sptr;
            dptr += 8;
            sptr += 8;
            size -= 8;
        }
    }

    if (size >= 4)
    {
        *(u32 *) dptr = *(u32 *) sptr;
        dptr += 4;
        sptr += 4;
        size -= 4;
    }
    if (size >= 2)
    {
        *(u16 *) dptr = *(u16 *) sptr;
        dptr += 2;
        sptr += 2;
        size -= 2;
    }
    if (size > 0)
    {
        *dptr = *sptr;
    }

    return dest;
}

void *memcpy(void *dest, const void *src, size_t size)
{
    if (size >= SMALL_SIZE)
        return (__asm_memcpy(dest, src, size));
    return (__inline_memcpy(dest, src, size));
}
