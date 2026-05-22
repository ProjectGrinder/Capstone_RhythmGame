#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "system/thread.h"


ThreadHandle thread_create(ThreadFunc func, void *args)
{
    DWORD thread_id;
    HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, args, 0, &thread_id);
    return ((ThreadHandle) thread);
}

void thread_join(ThreadHandle thread)
{
    if (thread)
    {
        WaitForSingleObject((HANDLE) thread, INFINITE);
        CloseHandle((HANDLE) thread);
    }
}

EventHandle event_create(void)
{
    HANDLE event = CreateEventA(NULL, FALSE, FALSE, NULL);

    return ((EventHandle) event);
}

void event_signal(EventHandle event)
{
    if (event)
    {
        SetEvent((HANDLE) event);
    }
}

void event_wait(EventHandle event)
{
    if (event)
    {
        WaitForSingleObject((HANDLE) event, INFINITE);
    }
}

void event_destroy(EventHandle event)
{
    if (event)
    {
        CloseHandle((HANDLE) event);
    }
}
