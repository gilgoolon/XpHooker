#pragma once
#include <memory>

class Process;

class HookedFunction final
{
public:
	explicit HookedFunction(std::weak_ptr<Process> process, void* new_function);
	~HookedFunction();
	HookedFunction(const HookedFunction&) = delete;
	HookedFunction& operator=(const HookedFunction&) = delete;
	HookedFunction(HookedFunction&&) = delete;
	HookedFunction& operator=(HookedFunction&&) = delete;

	static void* hook_function(Process& process, void* new_function);

private:
	std::weak_ptr<Process> m_process;
	void* m_previous_function;
};
