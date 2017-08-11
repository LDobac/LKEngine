#include "VulkanDebug.h"

#include "VulkanInstance.h"

void LKEngine::Vulkan::VulkanDebug::Init(VulkanInstance* vkInstance)
{
	instance = vkInstance;

	VkDebugReportCallbackCreateInfoEXT callbackInfo = {};
	callbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callbackInfo.pfnCallback = VulkanDebug::DebugCallback;
	callbackInfo.pUserData = nullptr;
	Check_Throw(CreateDebugCallback(instance->GetRawInstance(),&callbackInfo,nullptr,&callbackInstance) != VK_SUCCESS, "Vulkan Debug 셋팅 실패!");
}

void LKEngine::Vulkan::VulkanDebug::CleanUp()
{
	DestroyDebugCallback(instance->GetRawInstance(), callbackInstance, nullptr);
}

VkResult LKEngine::Vulkan::VulkanDebug::CreateDebugCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
{
	auto createDebug = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (createDebug != nullptr) 
	{
		return createDebug(instance, pCreateInfo, pAllocator, pCallback);
	}
	else 
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void LKEngine::Vulkan::VulkanDebug::DestroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto destoryDebug = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	Check_Warning(destoryDebug, "DestroyDebugReportCallback 호출 실패!");
	if (destoryDebug != nullptr) 
	{
		destoryDebug(instance, callback, pAllocator);
	}
}
