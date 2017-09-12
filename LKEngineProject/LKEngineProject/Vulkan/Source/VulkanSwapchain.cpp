#include "../Header/VulkanSwapchain.h"

using namespace LKEngine::Vulkan;

SwapchainSupportDetail::SwapchainSupportDetail(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	//해당 표면 기능 질의
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);

	//해당 표면에서의 포맷 갯수 질의
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		formats.resize(formatCount);
		//해당 표면의 포맷 데이터 질의
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, formats.data());
	}

	//해당 표면의 프레젠테이션 모드 갯수 질의
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		//해당 표면의 프레젠테이션 모드 질의
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes.data());
	}
}

bool SwapchainSupportDetail::CheckSwapchainAdequate()
{
	return !(formats.empty() || presentModes.empty());
}
