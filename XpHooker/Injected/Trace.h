#pragma once

#ifdef _DEBUG

#include <Windows.h>

#define TRACE(exp) OutputDebugStringW(L"[InjectedDll] " exp L"\n")
#define TRACE_VAR(exp) OutputDebugStringW(L"[InjectedDll] "); OutputDebugStringW(exp); OutputDebugStringW(L"\n")

#else

#define TRACE(exp) (exp);

#endif
