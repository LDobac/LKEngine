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
	std::vector<VulkanDescriptorSetLayout*> descriptorSetLayouts;
public:
	explicit VulkanGraphicsPipeline(VulkanShaderModule* vertShader, VulkanShaderModule* fragShader, const std::vector<VulkanDescriptorSetLayout*>& descriptorSetLayouts);
	virtual ~VulkanGraphicsPipeline();

	virtual void Recreate() override;

private:
	void CreatePipelineLayout();

	VkPipelineShaderStageCreateInfo ShaderStageInfo(VkShaderStageFlagBits stage, const VulkanShaderModule* module);
	VkPipelineVertexInputStateCreateInfo VertexInputInfo(const VkVertexInputBindingDescription& bindingDescription, const std::vector<VkVertexInputAttributeDescription>& attributeDescription);
	VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo(const VkPrimitiveTopology& topology);
	VkPipelineViewportStateCreateInfo ViewportStateInfo(const VkViewport& viewport, const VkRect2D& scissor);
	VkPipelineRasterizationStateCreateInfo RasterizerInfo(const VkCullModeFlags& cullmode, const VkFrontFace& frontface);
	VkPipelineMultisampleStateCreateInfo MultisamplingInfo();
	VkPipelineDepthStencilStateCreateInfo DepthStencilInfo();
	VkPipelineColorBlendStateCreateInfo ColorBlendingInfo(const VkPipelineColorBlendAttachmentState& attachment);
};

LK_VULKAN_SPACE_END