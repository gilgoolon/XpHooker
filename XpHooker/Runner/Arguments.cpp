#include "Arguments.hpp"

#include "CommandLineArguments.hpp"

enum Argument : uint32_t
{
	ARGUMENT_EXE_PATH = 0,

	ARGUMENT_DLL_PATH,
	ARGUMENT_PID_TO_INJECT,

	ARGUMENT_COUNT,
};

Arguments::Parsed Arguments::parse()
{
	const CommandLineArguments args(ARGUMENT_COUNT);
	return {args.wstring(ARGUMENT_DLL_PATH), args.uint32(ARGUMENT_PID_TO_INJECT)};
}
