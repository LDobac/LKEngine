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

	virtual void Recreate();

	void ReleasePipeline();

	const VkPipeline& GetHandle() const;
	const VkPipelineLayout& GetLayout() const;
};

class VulkanGraphicsPipeline
	: public VulkanPipeline
{
private:
	VulkanShaderModule* vertShader;
	VulkanShaderModule* fragShader;
	VulkanDescriptorSetLayout* descriptorSetLayout;
public:
	explicit VulkanGraphicsPipeline(VulkanShaderModule* vertShader, VulkanShaderModule* fragShader, VulkanDescriptorSetLayout* descriptorSetLayout);
	virtual ~VulkanGraphicsPipeline();

	virtual void Recreate() override;
};

LK_VULKAN_SPACE_END