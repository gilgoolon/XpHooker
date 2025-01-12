#include "Include/DynamicLibrary.hpp"

#include "Include/Exception.hpp"
#include "Include/Trace.hpp"

DynamicLibrary::DynamicLibrary(const std::filesystem::path& dll_path):
	m_module(load_library(dll_path))
{
}

DynamicLibrary::~DynamicLibrary()
{
	try
	{
		if (FreeLibrary(m_module) == FALSE)
		{
			TRACE(L"failed to free library")
		}
	}
	catch (...)
	{
		TRACE(L"failed to free library")
	}
}

HMODULE DynamicLibrary::load_library(const std::filesystem::path& dll_path)
{
	const HMODULE result = LoadLibraryW(dll_path.wstring().c_str());
	if (result == nullptr)
	{
		throw WinApiException(ErrorCode::FAILED_LIBRARY_LOAD);
	}
	return result;
}

void* DynamicLibrary::get_exported_procedure(const std::string& name) const
{
	const FARPROC result = GetProcAddress(m_module, name.c_str());
	if (result == nullptr)
	{
		throw WinApiException(ErrorCode::FAILED_LIBRARY_GET_PROC_ADDRESS);
	}
	return result;
}

PicComponent::Ptr DynamicLibrary::get_pic_component(const std::string& component_name)
{
	struct PublicPicComponent : PicComponent
	{
		PublicPicComponent(std::weak_ptr<DynamicLibrary> library, const std::string& component_name) :
			PicComponent(std::move(library), component_name)
		{
		}
	};
	return std::make_unique<PublicPicComponent>(weak_from_this(), component_name);
}
