#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanRenderPass;
class VulkanSwapchain;
class VulkanDescriptorSetLayout;
class VulkanShaderModule;

class VulkanPipeline
	: public VulkanDeviceChild
{
protected:
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
public:
	explicit VulkanPipeline();
	virtual ~VulkanPipeline();

	const VkPipeline& GetHandle() const;
	const VkPipelineLayout& GetLayout() const;
};

class VulkanGraphicsPipeline
	: public VulkanPipeline
{
public:
	explicit VulkanGraphicsPipeline(VulkanShaderModule* vertShader, VulkanShaderModule* fragShader, VulkanDescriptorSetLayout* descriptorSetLayout);
	virtual ~VulkanGraphicsPipeline();
};

LK_VULKAN_SPACE_END