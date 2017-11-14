#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>

#include "../../Utility/Header/Macro.h"

#include "VulkanBaseInterface.h"

//Foward Declaration
namespace LKEngine::Vulkan
{
	class VulkanInstance;
}

namespace LKEngine::Vulkan
{
	class VulkanDebug : public VulkanBaseInterface
	{
	private:
		VulkanInstance* instance;

		VkDebugReportCallbackEXT callbackInstance;
	public:
		explicit VulkanDebug(VulkanInstance* vkInstance);
		virtual ~VulkanDebug();

		virtual void Init() override;

		virtual void Shutdown() override;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) 
		{
			std::cerr << "validation layer: " << msg << std::endl;

			return VK_FALSE;
		}
	private:
		VkResult CreateDebugCallback(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
		void DestroyDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
	};
}