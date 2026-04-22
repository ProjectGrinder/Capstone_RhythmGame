#pragma once
typedef void *ThreadHandle;
typedef void *EventHandle;

typedef unsigned long (*ThreadFunc)(void *args);

#ifdef __cplusplus
extern "C"
{
#endif
    ThreadHandle thread_create(ThreadFunc func, void *args);
    void thread_join(ThreadHandle thread);

    EventHandle event_create(void);
    void event_signal(EventHandle event);
    void event_wait(EventHandle event);
    void event_destroy(EventHandle event);

#ifdef __cplusplus
}
#endif
