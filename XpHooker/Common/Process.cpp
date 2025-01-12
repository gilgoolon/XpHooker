#include "Include/Process.hpp"

#include "DynamicLibrary.hpp"
#include "Exception.hpp"
#include "LoadedPeModule.hpp"
#include "Strings.hpp"
#include "Trace.hpp"
#include "WinApi.hpp"

#include <Psapi.h>
#include <Windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

Process::Process(const uint32_t pid):
	m_handle(open_process(pid)),
	m_loaded_modules()
{
}

Process::Process(const std::wstring& command_line):
	m_handle(create_process(command_line)),
	m_loaded_modules()
{
}

HANDLE Process::open_process(const uint32_t pid)
{
	static constexpr DWORD READ_ACCESS = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | SYNCHRONIZE;
	static constexpr BOOL DONT_INHERIT_HANDLE = FALSE;
	const HANDLE result = OpenProcess(READ_ACCESS, DONT_INHERIT_HANDLE, pid);
	if (result == nullptr)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_OPEN);
	}
	return result;
}

HANDLE Process::create_process(const std::wstring& command_line)
{
	static constexpr LPCWSTR NAME_FROM_COMMAND_LINE = nullptr;
	static constexpr LPSECURITY_ATTRIBUTES DEFAULT_SECURITY = nullptr;
	static constexpr BOOL DONT_INHERIT_HANDLE = FALSE;
	static constexpr DWORD RUN_ON_CREATION = 0;
	static constexpr LPVOID THIS_PROCESS_ENVIRONMENT = nullptr;
	static constexpr LPCWSTR THIS_PROCESS_CWD = nullptr;

	std::wstring command_line_copy = command_line;
	STARTUPINFOW startup_info{};
	PROCESS_INFORMATION process_information{};
	const BOOL result = CreateProcessW(
		NAME_FROM_COMMAND_LINE,
		command_line_copy.data(),
		DEFAULT_SECURITY,
		DEFAULT_SECURITY,
		DONT_INHERIT_HANDLE,
		RUN_ON_CREATION,
		THIS_PROCESS_ENVIRONMENT,
		THIS_PROCESS_CWD,
		&startup_info,
		&process_information
	);
	if (result == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_CREATE);
	}
	ScopedHandle thread_handle_guard(process_information.hThread);
	return process_information.hProcess;
}

using NtQueryInformationProcessFunc = NTSTATUS(WINAPI*)(
	HANDLE process_handle,
	PROCESSINFOCLASS process_information_class,
	PVOID process_information,
	ULONG process_information_length,
	PULONG return_length);

PROCESS_BASIC_INFORMATION Process::query_basic_information() const
{
	const DynamicLibrary loaded_dll("ntdll.dll");
	PROCESS_BASIC_INFORMATION pbi;
	ULONG return_length = 0;
	const NTSTATUS result = loaded_dll.call<NtQueryInformationProcessFunc>(
		"NtQueryInformationProcess",
		m_handle.get(),
		ProcessBasicInformation,
		&pbi,
		static_cast<ULONG>(sizeof(pbi)),
		&return_length
	);
	if (!NT_SUCCESS(result))
	{
		throw WinApiNtException(ErrorCode::FAILED_PROCESS_QUERY_INFORMATION, result);
	}
	return pbi;
}

uint32_t Process::get_pid() const
{
	const uint32_t result = GetProcessId(m_handle.get());
	if (result == 0)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_PID);
	}
	return result;
}

std::string Process::get_path() const
{
	static constexpr HMODULE MAIN_MODULE_EXE = nullptr;
	static constexpr char NULL_TERMINATOR = '\0';
	std::string path(MAX_PATH, NULL_TERMINATOR);
	const DWORD result = GetModuleFileNameExA(
		m_handle.get(),
		MAIN_MODULE_EXE,
		path.data(),
		path.size()
	);
	if (result == 0)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_PATH);
	}
	path.resize(result);
	return path;
}

bool Process::is_running() const
{
	const WinApi::WaitStatus result = WinApi::wait(m_handle.get(), Time::INSTANT);
	switch (result)
	{
	case WinApi::WaitStatus::FINISHED:
		[[fallthrough]];
	case WinApi::WaitStatus::OBJECT_CLOSED:
		return true;

	case WinApi::WaitStatus::TIMEOUT:
		return false;

	case WinApi::WaitStatus::FAILED:
		[[fallthrough]];
	default:
		throw WinApiException(ErrorCode::FAILED_WAIT);
	}
}

Time::Datetime Process::get_creation_time() const
{
	FILETIME creation_time{};
	FILETIME exit_time{};
	FILETIME kernel_time{};
	FILETIME user_time{};
	if (GetProcessTimes(
		m_handle.get(),
		&creation_time,
		&exit_time,
		&kernel_time,
		&user_time
	) == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_TIMES);
	}
	return Time::to_datetime(creation_time);
}

std::string Process::get_command_line() const
{
	const PROCESS_BASIC_INFORMATION basic_information = query_basic_information();

	const Buffer raw_peb = read_memory(basic_information.PebBaseAddress, sizeof(PEB));
	const auto* const peb = reinterpret_cast<const PEB*>(raw_peb.data());

	const Buffer raw_process_parameters = read_memory(peb->ProcessParameters, sizeof(RTL_USER_PROCESS_PARAMETERS));
	const auto* const process_parameters = reinterpret_cast<const RTL_USER_PROCESS_PARAMETERS*>(raw_process_parameters.
		data());

	const Buffer raw_command_line = read_memory(
		process_parameters->CommandLine.Buffer,
		process_parameters->CommandLine.Length
	);
	return Strings::to_string(Strings::to_wstring(raw_command_line));
}

void Process::terminate()
{
	static constexpr DWORD EXIT_CODE = EXIT_FAILURE;
	if (TerminateProcess(m_handle.get(), EXIT_CODE) == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_TERMINATE);
	}
}

Buffer Process::read_memory(const void* const address, const size_t size) const
{
	Buffer buffer(size);
	SIZE_T bytes_read = 0;
	const BOOL result = ReadProcessMemory(m_handle.get(), address, buffer.data(), size, &bytes_read);
	if (result == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_READ_MEMORY);
	}
	return buffer;
}

RemoteMemory::Ptr Process::allocate_memory(uint32_t size)
{
	struct PublicRemoteMemory : RemoteMemory
	{
		PublicRemoteMemory(std::weak_ptr<Process> process, const uint32_t size):
			RemoteMemory(std::move(process), size)
		{
		}
	};
	return std::make_unique<PublicRemoteMemory>(weak_from_this(), size);
}
