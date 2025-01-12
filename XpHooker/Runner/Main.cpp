#include "Arguments.hpp"
#include "Configuration.hpp"
#include "DynamicLibrary.hpp"
#include "Exception.hpp"
#include "Process.hpp"
#include "ProcessHooker.hpp"
#include "Strings.hpp"
#include "Trace.hpp"
#include "WinApi.hpp"

#include <Windows.h>

static void main_logic(const Arguments::Parsed& args)
{
	auto process = std::make_shared<Process>(L"notepad.exe");
	PicComponent::Ptr load_dll_pic = DynamicLibrary(std::wstring{Configuration::STUBBED_DLL_PATH}).
		get_pic_component(std::string{Configuration::LOAD_DLL_COMPONENT});
	HookedFunction::Ptr hooked_function = ProcessHooker::hook(
		*process,
		std::string{Configuration::HOOKED_FUNCTION},
		*load_dll_pic
	);
	WinApi::sleep(Time::Seconds(5));
	process->terminate();
	TRACE(L"finished successfully")
}

int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance,
                    [[maybe_unused]] HINSTANCE hPrevInstance,
                    [[maybe_unused]] PWSTR pCmdLine,
                    [[maybe_unused]] int nCmdShow)
{
	try
	{
		const Arguments::Parsed args = Arguments::parse();
		main_logic(args);
		return EXIT_SUCCESS;
	}
	catch (const WinApiException& ex)
	{
		TRACE(L"uncaught WinApiException code=", ex.code(), ", error=", ex.error())
	}
	catch (const Exception& ex)
	{
		TRACE(L"uncaught Exception code=", ex.code())
	}
	catch (const std::exception& ex)
	{
		TRACE(L"uncaught std::exception what=", ex.what())
	}
	catch (...)
	{
		TRACE(L"uncaught unknown exception")
	}

	return EXIT_FAILURE;
}
