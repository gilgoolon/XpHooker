#pragma once
#include <cstdint>
#include <memory>

class Process;

class RemoteMemory
{
	explicit RemoteMemory(std::weak_ptr<Process> process, uint32_t size);

public:
	using Ptr = std::unique_ptr<RemoteMemory>;
	~RemoteMemory();
	RemoteMemory(const RemoteMemory&) = delete;
	RemoteMemory& operator=(const RemoteMemory&) = delete;
	RemoteMemory(RemoteMemory&&) = delete;
	RemoteMemory& operator=(RemoteMemory&&) = delete;

private:
	std::weak_ptr<Process> m_process;
	void* m_base_address;

	[[nodiscard]] static void* allocate_remote_memory(Process& process, uint32_t size);
};
