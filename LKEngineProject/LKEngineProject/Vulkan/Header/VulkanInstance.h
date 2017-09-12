#pragma once

#include <vulkan/vulkan.h>

#include "VulkanBaseInterface.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanExtension;
		class VulkanDebug;

		class VulkanInstance
			: public BaseInterface
		{
		private:
			VkInstance vkInstance;

			VulkanDebug* debug;
		public :
			explicit VulkanInstance();
			~VulkanInstance();

			void Init(bool vaildationLayerOn);
			virtual void Shutdown() override;

			VkInstance GetRawInstance() const;
		};
	}
}