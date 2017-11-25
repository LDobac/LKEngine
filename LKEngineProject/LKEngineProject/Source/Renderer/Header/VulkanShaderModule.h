#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanShaderModule
{
public:
	enum class ShaderType
	{
		VERTEX,
		FRAGMENT
	};

private:
	ShaderType shaderType;

	VkShaderModule shaderModule;
public:
	VulkanShaderModule(const ShaderType type, const std::string & shaderPath);
	~VulkanShaderModule();

	VkShaderModule GetHandle() const;
	ShaderType GetShaderType() const;

private:
	void Init(const ShaderType type, const std::vector<char>& compileCode);

	std::vector<char> ReadCompiledShader(const std::string& file);
};

LK_VULKAN_SPACE_END