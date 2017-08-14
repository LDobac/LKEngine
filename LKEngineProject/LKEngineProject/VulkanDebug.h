#pragma once

#include <vulkan/vulkan.hpp>
#include <iostream>

#include "Macro.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanInstance;

		class VulkanDebug
		{
		private:
			VulkanInstance* instance;

			VkDebugReportCallbackEXT callbackInstance;
		public:
			explicit VulkanDebug(VulkanInstance* vkInstance);
			
			~VulkanDebug();

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
}