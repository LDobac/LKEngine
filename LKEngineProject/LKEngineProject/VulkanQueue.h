#pragma once

#include <vulkan/vulkan.h>

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice;

		class VulkanQueue
		{
		private:
			VulkanDevice* device;

			VkQueue vkQueue;
		public:
			explicit VulkanQueue(VulkanDevice* device, uint32_t familyIndex, uint32_t queueIndex);
		};
	}
}