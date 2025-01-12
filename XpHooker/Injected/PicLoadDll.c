#include "PicLoadDll.h"

#include "Trace.h"

LoadDllStatusCode PIC__A__load_dll__START(const wchar_t* dll_path)
{
	TRACE(L"loading dll");

	// TODO: load a dll from a path in constant Stack memory

	return TRUE;
}

void PIC__B__load_dll__END(void)
{
}
