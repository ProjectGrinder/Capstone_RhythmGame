#pragma once
#include <Windows.h>
#include "windows_types.h"

DWORD __vectorcall create_window(_In_ SystemInfo *system_info);
void __vectorcall sleep_init(void);
void __vectorcall process_system_message(_In_ SystemInfo *system_info, _In_ MSG *msg);
void __vectorcall sleep(_In_ LONGLONG amount);