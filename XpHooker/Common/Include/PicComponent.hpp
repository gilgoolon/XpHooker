#pragma once
#include <memory>
#include <string>
#include <vector>

class PicComponent
{
	friend class DynamicLibrary;
	friend class Process;

	explicit PicComponent(const DynamicLibrary& library, const std::string& component_name);

public:
	using Ptr = std::unique_ptr<PicComponent>;
	~PicComponent() = default;
	PicComponent(const PicComponent&) = delete;
	PicComponent& operator=(const PicComponent&) = delete;
	PicComponent(PicComponent&&) = delete;
	PicComponent& operator=(PicComponent&&) = delete;

private:
	std::vector<uint8_t> m_data;

	[[nodiscard]] std::vector<uint8_t> get() const;

	[[nodiscard]] static std::vector<uint8_t> get_pic_data(const DynamicLibrary& library,
	                                                       const std::string& component_name);

	static constexpr std::string_view COMPONENT_PREFIX = "PIC__";

	[[nodiscard]] static std::string get_start_function_name(const std::string& component_name);

	[[nodiscard]] static std::string get_end_function_name(const std::string& component_name);
};
