#pragma once
#include <Windows.h>

typedef enum
{
	LD_SUCCESS = 0,
	LD_FAILURE,
	LD_CRITICAL_FAILURE,
} LoadDllStatusCode;

__declspec(dllexport) LoadDllStatusCode __cdecl PIC__A__load_dll__START(const wchar_t* dll_path);

__declspec(dllexport) void __cdecl PIC__B__load_dll__END(void);
