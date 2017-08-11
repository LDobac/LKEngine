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
			void Init(bool vaildationLayerOn);

			void CleanUp();

			VkInstance GetRawInstance() const;
		};
	}
}