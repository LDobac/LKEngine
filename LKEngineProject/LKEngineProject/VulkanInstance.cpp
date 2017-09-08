#include "VulkanInstance.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "VulkanExtension.h"
#include "Macro.h"
#include "VulkanDebug.h"

LKEngine::Vulkan::VulkanInstance::VulkanInstance(bool vaildationLayerOn)
	:vkInstance(VK_NULL_HANDLE),
	debug(nullptr),
	extension(nullptr)
{
	Console_Log_If(vaildationLayerOn, "디버그 활성화");

	glfwInit();

	extension = new VulkanExtension(vaildationLayerOn);

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Unnamed";
	appInfo.applicationVersion = APPLICATION_VERSION;
	appInfo.pEngineName = ENGINE_NAME;
	appInfo.engineVersion = LK_VERSION;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extension->GetExtensions().size());
	std::vector<const char*> extensions = extension->GetExtensions();
	createInfo.ppEnabledExtensionNames = extensions.data();

	if (vaildationLayerOn)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(vaildationLayers.size());
		createInfo.ppEnabledLayerNames = vaildationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	Check_Throw(VK_SUCCESS != vkCreateInstance(&createInfo, nullptr, &vkInstance), "VkInstance 생성 실패!");

	if (vaildationLayerOn)
	{
		debug = new VulkanDebug(this);
	}

	Console_Log("Vulkan Instance 생성 완료");
}

LKEngine::Vulkan::VulkanInstance::~VulkanInstance()
{
	delete extension;

	delete debug;
	debug = nullptr;

	vkDestroyInstance(vkInstance, nullptr);
}

VkInstance LKEngine::Vulkan::VulkanInstance::GetRawInstance() const
{
	return vkInstance;
}
