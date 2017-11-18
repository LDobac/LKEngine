#pragma once

#include <vulkan/vulkan.h>

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSwapchain;

	class VulkanRenderPass
		: public IVulkanObject, VulkanDeviceChild
	{
	private:
		VkRenderPass renderPass;
	public:
		explicit VulkanRenderPass(VulkanDevice* device);
		virtual ~VulkanRenderPass();

		virtual void Init() { }
		void Init(VulkanSwapchain* swapchain);
		virtual void Shutdown() override;

		void Begin(std::vector<VkClearValue> clearColors, const VkFramebuffer& frameBuffer, VkExtent2D extent, const VkCommandBuffer& cmdBuffer);
		void End(const VkCommandBuffer& cmdBuffer);

		VkRenderPass GetHandle() const;
	};
}