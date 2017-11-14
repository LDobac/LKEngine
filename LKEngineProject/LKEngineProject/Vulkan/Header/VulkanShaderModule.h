#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanShaderModule
		: public VulkanDeviceChild, VulkanBaseInterface
	{
	private:
		VkShaderModule shaderModule;
	public:
		VulkanShaderModule(VulkanDevice* device);
		~VulkanShaderModule();

		void Init(const std::string& shaderPath);
		void Init(const std::vector<char> compileCode);

		virtual void Shutdown() override;

		VkShaderModule GetHandle() const;
	};
}