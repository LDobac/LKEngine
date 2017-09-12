#pragma once

#include "VulkanBaseInterface.h"
#include "VulkanInstance.h"

namespace LKEngine
{
	namespace Window
	{
		class Window;
	}

	namespace Vulkan
	{
		class VulkanQueue;

		class VulkanDevice : public BaseInterface
		{
		private:
			VulkanInstance* instance;

			VkDevice vkDevice;

			VkPhysicalDevice gpu;
			VkPhysicalDeviceProperties gpuProp;

			VkSurfaceKHR surface;

			VulkanQueue* graphicsQueue;
			VulkanQueue* presentQueue;
		public:
			explicit VulkanDevice();
			virtual ~VulkanDevice();

			void Init(Window::Window* window, bool debug);
			virtual void Shutdown() override;

			VkDevice GetRawDevice() const;

		private:
			void CreateSurface(Window::Window* window);
			void RequirePhysicalDevice();
			void CreateDevice(bool vaildationLayerOn);

			bool CheckDeviceFeatures(VkPhysicalDevice device);
		};
	}
}