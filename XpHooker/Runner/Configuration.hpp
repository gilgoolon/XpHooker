#pragma once
#include <string>

namespace Configuration
{
static constexpr std::wstring_view STUBBED_DLL_PATH = L"../Debug/Injected.dll";
static constexpr std::string_view LOAD_DLL_COMPONENT = "load_dll";
static constexpr std::string_view HOOKED_FUNCTION = "FILL";
}
