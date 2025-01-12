#include "Include/ScopedLocalHeapAllocation.hpp"

#include "Include/Trace.hpp"

void LocalAllocationFreer::operator()(const HLOCAL address) const
{
	try
	{
		if (LocalFree(address) != nullptr)
		{
			TRACE(L"failed to free local allocation")
		}
	}
	catch (...)
	{
		TRACE(L"failed to free local allocation")
	}
}
