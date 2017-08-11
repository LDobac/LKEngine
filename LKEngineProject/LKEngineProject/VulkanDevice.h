#pragma once

#include "VulkanInstance.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice
		{
		private:
			VulkanInstance* vkInstance;

			VkDevice vkDevice;
			VkPhysicalDevice gpu;

		public:
			void Init();

			void CleanUp();
		};
	}
}