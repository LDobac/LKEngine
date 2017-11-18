#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanDescriptorSetLayout
		: public IVulkanObject
	{
	private:
		VkDescriptorSetLayout seyLayout;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	public:
		explicit VulkanDescriptorSetLayout(VulkanDevice* device);

		virtual void Init() override;
		virtual void Shutdown() override;

		void AddDescriptor(VkShaderStageFlagBits stageFlag, const VkDescriptorSetLayoutBinding& binding, int index);
		void CreateDescriptorSetLayout();
	};
}