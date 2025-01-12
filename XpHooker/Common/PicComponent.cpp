#include "PicComponent.hpp"
#include "DynamicLibrary.hpp"

PicComponent::PicComponent(const DynamicLibrary& library, const std::string& component_name):
	m_data(get_pic_data(library, component_name))
{
}

std::vector<uint8_t> PicComponent::get() const
{
	return m_data;
}

std::vector<uint8_t> PicComponent::get_pic_data(const DynamicLibrary& library, const std::string& component_name)
{
	void* start_function = library.get_exported_procedure(get_start_function_name(component_name));
	void* end_function = library.get_exported_procedure(get_end_function_name(component_name));
	return {static_cast<uint8_t*>(start_function), static_cast<uint8_t*>(end_function)};
}

std::string PicComponent::get_start_function_name(const std::string& component_name)
{
	static constexpr std::string_view START_SUFFIX = "__START";
	return std::string{COMPONENT_PREFIX} + component_name + std::string{START_SUFFIX};
}

std::string PicComponent::get_end_function_name(const std::string& component_name)
{
	static constexpr std::string_view END_SUFFIX = "__END";
	return std::string{COMPONENT_PREFIX} + component_name + std::string{END_SUFFIX};
}
