#pragma once

#include <vulkan/vulkan.h>

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanExtension;
		class VulkanDebug;

		class VulkanInstance
		{
		private:
			VkInstance vkInstance;

			VulkanExtension* extension;
			VulkanDebug* debug;
		public :
			explicit VulkanInstance(bool vaildationLayerOn);

			~VulkanInstance();

			VkInstance GetRawInstance() const;
		};
	}
}