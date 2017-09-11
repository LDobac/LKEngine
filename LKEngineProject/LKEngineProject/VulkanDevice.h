#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanInstance.h"

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice : public BaseInterface
		{
		private:
			VulkanInstance* instance;

			VkDevice vkDevice;
			VkPhysicalDevice gpu;
		public:
			explicit VulkanDevice();
			virtual ~VulkanDevice();

			void Init(bool debug);
			virtual void Shutdown() override;

			VkDevice GetRawDevice() const;
		private:
			void RequirePhysicalDevice();

			bool CheckDeviceFeatures(VkPhysicalDevice device);
		};
	}
}