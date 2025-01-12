#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE instance,
                    DWORD reason,
                    LPVOID reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);
	}
	return TRUE;
}
