#pragma once

#include <vulkan/vulkan.h>

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanSwapchain;

		class VulkanRenderPass
			: public BaseInterface, VulkanDeviceChild
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
}