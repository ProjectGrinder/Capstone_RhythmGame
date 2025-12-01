#pragma once
#include <Windows.h>
#define INLINEOPTIMIZE static inline __attribute__((always_inline)) __forceinline

typedef enum
{
    DT_WINDOW,
    DT_BORDERLESS,
    DT_FULLSCREEN,
} DisplayType;

typedef struct
{
    LONG width;
    LONG height;
} Window;

typedef struct
{
    LONG width;
    LONG height;
} Monitor;

typedef struct
{
    INT x;
    INT y;
} Position;

typedef struct
{
    void *addr[256];
} SystemQueue;

typedef SystemQueue VertexQueue;
typedef SystemQueue RenderingQueue;

typedef struct
{
    Window window;
    Monitor monitor;

    HINSTANCE instance_handler;
    HWND window_handler;

    DisplayType display_type;
    char is_running;

    LONGLONG precision;

    VertexQueue *vertex_queue;
    RenderingQueue *rendering_queue;
} SystemInfo;
