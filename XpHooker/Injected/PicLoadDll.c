#include "PicLoadDll.h"

#include "Trace.h"

LoadDllStatusCode PIC__load_dll__START(const wchar_t* dll_path)
{
	TRACE(L"loading dll");
	return TRUE;
}

void PIC__load_dll__END(void)
{
}
