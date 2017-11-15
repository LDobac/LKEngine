#include "../Header/VulkanDebug.h"

#include "../../Utility/Header/Macro.h"
#include "../Header/VulkanInstance.h"

using namespace LKEngine::Vulkan;

VulkanDebug::VulkanDebug(VulkanInstance* vkInstance)
	:instance(vkInstance),
	callbackInstance(VK_NULL_HANDLE) 
{ }

VulkanDebug::~VulkanDebug() { }

void VulkanDebug::Init()
{
	Console_Log("Vaildation Layer 생성 시작");

	VkDebugReportCallbackCreateInfoEXT callbackInfo = {};
	callbackInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callbackInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callbackInfo.pfnCallback = VulkanDebug::DebugCallback;
	callbackInfo.pUserData = nullptr;

	VkResult result = CreateDebugCallback(*(*instance), &callbackInfo, nullptr, &callbackInstance);
	Check_Throw(result != VK_SUCCESS, "Vaildation Layer 생성 실패!");

	Console_Log("Vaildation Layer 생성 완료");
}

void VulkanDebug::Shutdown()
{
	DestroyDebugCallback(*(*instance), callbackInstance, nullptr);
}

VkResult VulkanDebug::CreateDebugCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT * pCreateInfo, const VkAllocationCallbacks * pAllocator, VkDebugReportCallbackEXT * pCallback)
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

void VulkanDebug::DestroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
	auto destoryDebug = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	Check_Warning(destoryDebug == nullptr, "DestroyDebugReportCallback 호출 실패!");
	if (destoryDebug != nullptr) 
	{
		destoryDebug(instance, callback, pAllocator);
	}
}
