#pragma once
#include "RemoteMemory.hpp"

#include <memory>
#include <string>

class Process;

class HookedFunction final
{
public:
	using Ptr = std::unique_ptr<HookedFunction>;
	explicit HookedFunction(std::weak_ptr<Process> process,
	                        const std::string& function_name,
	                        RemoteMemory::Ptr new_function);
	~HookedFunction();
	HookedFunction(const HookedFunction&) = delete;
	HookedFunction& operator=(const HookedFunction&) = delete;
	HookedFunction(HookedFunction&&) = delete;
	HookedFunction& operator=(HookedFunction&&) = delete;

	static void* hook_function(Process& process, const std::string& function_name, void* new_function);

private:
	std::weak_ptr<Process> m_process;
	std::string m_function_name;
	RemoteMemory::Ptr m_new_function;
	void* m_previous_function;
};
