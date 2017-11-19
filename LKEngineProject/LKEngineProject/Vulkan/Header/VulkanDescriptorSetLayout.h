#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanDescriptorSetLayout
		: public VulkanDeviceChild
	{
	private:
		VkDescriptorSetLayout setLayout;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	public:
		explicit VulkanDescriptorSetLayout(VulkanDevice* device);

		virtual void Shutdown();

		void AddDescriptor(VkDescriptorType type, VkShaderStageFlagBits stageFlag, int index);
		void CreateDescriptorSetLayout();

		const VkDescriptorSetLayout& GetHandle() const;
	};
}