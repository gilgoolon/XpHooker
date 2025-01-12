#include "HookedFunction.hpp"

#include "Trace.hpp"

HookedFunction::HookedFunction(std::weak_ptr<Process> process,
                               const std::string& function_name,
                               RemoteMemory::Ptr new_function):
	m_process(std::move(process)),
	m_function_name(function_name),
	m_new_function(std::move(new_function)),
	m_previous_function(hook_function(*m_process.lock(), function_name, m_new_function->get()))
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
	// TODO: do actual PE IAT hooking logic
	return nullptr;
}
