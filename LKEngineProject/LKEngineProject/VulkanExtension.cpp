#include "VulkanExtension.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Macro.h"

using namespace LKEngine;

Vulkan::VulkanExtension::VulkanExtension(bool isDebug)
{
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
}

std::vector<const char*> Vulkan::VulkanExtension::GetExtensions() const
{
	return vaildExtensions;
}
