#pragma once

#include <vulkan/vulkan.h>

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"

namespace LKEngine::Vulkan
{
	class VulkanSwapchain;

	class VulkanRenderPass
		: public VulkanBaseInterface, VulkanDeviceChild
	{
	private:
		VkRenderPass renderPass;
	public:
		explicit VulkanRenderPass(VulkanDevice* device);
		virtual ~VulkanRenderPass();

		void Init(VulkanSwapchain* swapchain);
		virtual void Shutdown() override;
	};
}