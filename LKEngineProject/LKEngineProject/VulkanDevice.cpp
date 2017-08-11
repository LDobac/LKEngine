#include "VulkanDevice.h"

void LKEngine::Vulkan::VulkanDevice::Init()
{
	vkInstance = new VulkanInstance();
	vkInstance->Init(true);
}

void LKEngine::Vulkan::VulkanDevice::CleanUp()
{
	vkInstance->CleanUp();
	delete vkInstance;
	vkInstance = nullptr;
}
