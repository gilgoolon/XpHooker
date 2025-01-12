#pragma once
#include <cstdint>

namespace Arguments
{
struct Parsed final
{
	uint32_t pid_to_inject;
};

[[nodiscard]] Parsed parse();
};
