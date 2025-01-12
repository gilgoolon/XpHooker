#pragma once
#include <cstdint>
#include <string>

namespace Arguments
{
struct Parsed final
{
	std::wstring dll_to_inject;
	uint32_t pid_to_inject;
};

[[nodiscard]] Parsed parse();
};
