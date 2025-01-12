#include "ProcessHooker.hpp"

#include "Process.hpp"
#include "RemoteMemory.hpp"

HookedFunction::Ptr ProcessHooker::hook(Process& process, const std::string& function_name, const PicComponent& pic)
{
	RemoteMemory::Ptr pic_memory = process.allocate_memory(pic.size());
	pic_memory->write(pic.data());
	return std::make_unique<HookedFunction>(process.weak_from_this(), function_name, std::move(pic_memory));
}
