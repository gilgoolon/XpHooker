#include "ProcessHooker.hpp"

#include "Process.hpp"
#include "RemoteMemory.hpp"

HookedFunction::Ptr ProcessHooker::hook(Process& process, const std::string& function_name, const PicComponent& pic)
{
	const RemoteMemory::Ptr pic_memory = process.allocate_memory(pic.size());
	pic_memory->write(pic.data());
}
