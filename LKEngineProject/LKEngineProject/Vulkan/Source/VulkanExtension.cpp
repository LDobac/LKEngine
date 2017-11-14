#include "../Header/VulkanExtension.h"

#include <GLFW/glfw3.h>
#include <set>

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

std::vector<const char*> VulkanExtension::GetInstanceExtensions(bool isDebug) const
{
	std::vector<const char*> vaildExtensions;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	Check_Throw(0 == glfwExtensionCount, "사용 가능한 확장의 개수가 0 입니다!");

	vaildExtensions.resize(glfwExtensionCount);
	for (uint32_t i = 0; i < glfwExtensionCount; i++)
	{
		vaildExtensions[i] = glfwExtensions[i];
	}

	if (isDebug)
	{
		vaildExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return vaildExtensions;
}

bool LKEngine::Vulkan::VulkanExtension::CheckDeviceExtensionSupport(VkPhysicalDevice gpu)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(gpu, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (size_t i = 0; i < availableExtensions.size() ; i++)
	{
		requiredExtensions.erase(availableExtensions[i].extensionName);
	}

	return requiredExtensions.empty();
}
