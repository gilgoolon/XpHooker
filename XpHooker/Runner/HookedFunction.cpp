#include "HookedFunction.hpp"

#include "Trace.hpp"

HookedFunction::HookedFunction(std::weak_ptr<Process> process, const std::string& function_name, void* new_function):
	m_process(std::move(process)),
	m_previous_function(hook_function(*m_process.lock(), function_name, new_function))
{
}

HookedFunction::~HookedFunction()
{
	try
	{
		hook_function(*m_process.lock(), m_function_name, m_previous_function);
	}
	catch (...)
	{
		TRACE(L"failed to remove hook back to old function")
	}
}

void* HookedFunction::hook_function(Process& process, const std::string& function_name, void* new_function)
{
	return nullptr;
}
