#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanRenderPass;
class VulkanSwapchain;
class VulkanDescriptorSetLayout;

class VulkanPipeline
	: public VulkanDeviceChild
{
protected:
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
public:
	explicit VulkanPipeline(VulkanDevice* device);

	virtual void Shutdown();

	const VkPipeline& GetHandle() const;
	const VkPipelineLayout& GetLayout() const;
};

class VulkanGraphicsPipeline
	: public VulkanPipeline
{
public:
	explicit VulkanGraphicsPipeline(VulkanDevice* device);

	void Init(VulkanRenderPass* renderPass, VulkanSwapchain* swapchain, VulkanDescriptorSetLayout* descriptorSetLayout);
};

LK_VULKAN_SPACE_END