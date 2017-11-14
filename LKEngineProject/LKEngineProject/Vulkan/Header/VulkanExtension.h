#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace LKEngine::Vulkan
{
	const std::vector<const char*> vaildationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	class VulkanExtension
	{
	public:
		explicit VulkanExtension() {}

		std::vector<const char*> GetInstanceExtensions(bool isDebug) const;

		bool CheckDeviceExtensionSupport(VkPhysicalDevice gpu);
	};
}