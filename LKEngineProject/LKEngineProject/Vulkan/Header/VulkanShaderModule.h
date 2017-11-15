#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanShaderModule
		: public VulkanDeviceChild, IVulkanObject
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
		VulkanShaderModule(VulkanDevice* device);
		~VulkanShaderModule();

		virtual void Init() override {} 
		void Init(const ShaderType type, const std::string& shaderPath);
		void Init(const ShaderType type, const std::vector<char>& compileCode);

		virtual void Shutdown() override;

		std::vector<char> ReadCompiledShader(const std::string& file);

		VkShaderModule GetHandle() const;
		ShaderType GetShaderType() const;
	};
}