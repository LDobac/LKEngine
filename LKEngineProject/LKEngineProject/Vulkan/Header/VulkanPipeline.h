#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanRenderPass;
	class VulkanSwapchain;
	class VulkanDescriptorSetLayout;

	class VulkanPipeline
		: public IVulkanObject, public VulkanDeviceChild
	{
	protected:
		VkPipeline pipeline;
		VkPipelineLayout pipelineLayout;
	public:
		explicit VulkanPipeline(VulkanDevice* device);

		virtual void Init() override { }
		virtual void Shutdown() override;

		const VkPipeline& GetHandle() const;
	};

	class VulkanGraphicsPipeline
		: public VulkanPipeline
	{
	public:
		explicit VulkanGraphicsPipeline(VulkanDevice* device);

		void Init(VulkanRenderPass* renderPass, VulkanSwapchain* swapchain, VulkanDescriptorSetLayout* descriptorSetLayout);
	};
}