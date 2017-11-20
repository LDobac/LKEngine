#pragma once

#include <vulkan/vulkan.hpp>
#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

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

LK_VULKAN_SPACE_END