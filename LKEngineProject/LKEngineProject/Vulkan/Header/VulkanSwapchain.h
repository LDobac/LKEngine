#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanBaseInterface.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class SwapchainSupportDetail
		{
		private:
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;

		public:
			explicit SwapchainSupportDetail(VkPhysicalDevice gpu,VkSurfaceKHR surface);

			bool CheckSwapchainAdequate();
		};

		class VulkanSwapchain
			: public BaseInterface
		{

		};
	}
}