#pragma once

#include "VulkanInstance.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice
		{
		private:
			VulkanInstance* instance;

			VkDevice vkDevice;
			VkPhysicalDevice gpu;

		public:
			explicit VulkanDevice();

			~VulkanDevice();

		private:
			void GetPhysicalDevice();

			bool CheckDeviceFeatures(VkPhysicalDevice device);
		};
	}
}