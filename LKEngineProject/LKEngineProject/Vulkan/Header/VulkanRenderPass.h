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

		VkRenderPass GetHandle() const;
	};
}