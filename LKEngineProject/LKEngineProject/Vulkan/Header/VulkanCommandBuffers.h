#pragma once

#include "IVulkanObject.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSwapchain;
	class VulkanCommandPool;

	class VulkanCommandBuffers
		: public IVulkanObject, public VulkanDeviceChild
	{
	private:
		VulkanCommandPool* commandPool;

		std::vector<VkCommandBuffer> commandBuffers;
	public:
		explicit VulkanCommandBuffers(VulkanDevice* device, VulkanCommandPool* commandPool);

		virtual void Init() override { }
		void Init(VulkanSwapchain* swapchain);
		virtual void Shutdown() override;
	};
}