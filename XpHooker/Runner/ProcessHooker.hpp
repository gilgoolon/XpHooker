#pragma once
#include "HookedFunction.hpp"
#include "PicComponent.hpp"

namespace ProcessHooker
{
[[nodiscard]] HookedFunction::Ptr hook(Process& process, const std::string& function_name, const PicComponent& pic);
};
