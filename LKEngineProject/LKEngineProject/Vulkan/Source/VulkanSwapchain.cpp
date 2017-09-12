#include "../Header/VulkanSwapchain.h"

using namespace LKEngine::Vulkan;

SwapchainSupportDetail::SwapchainSupportDetail(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	//서페이스 기능 질의
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);

	//서페이스 포맷 질의
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, formats.data());
	}

	//서페이스 프레젠트 모드 질의
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) 
	{
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes.data());
	}
}

bool SwapchainSupportDetail::CheckSwapchainAdequate()
{
	return !(formats.empty() || presentModes.empty());
}
