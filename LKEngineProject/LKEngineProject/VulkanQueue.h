#pragma once

#include <vulkan/vulkan.h>

#include "VulkanDeviceChild.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanQueue
			: VulkanDeviceChild
		{
		private:
			VkQueue vkQueue;
		public:
			explicit VulkanQueue(VulkanDevice* device, uint32_t familyIndex, uint32_t queueIndex);
		};
	}
}