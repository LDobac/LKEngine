#pragma once

#include <vector>

#include "VulkanBaseInterface.h"
#include "VulkanInstance.h"

//Foward Declaration
namespace LKEngine::Window
{
	class Window;
}

namespace LKEngine::Vulkan
{
	class VulkanQueue;
	class VulkanSwapchain;
	class VulkanRenderPass;

	class VulkanDevice 
		: public VulkanBaseInterface
	{
	private:
		VulkanInstance* instance;

		VkDevice vkDevice;

		VkPhysicalDevice gpu;
		VkPhysicalDeviceProperties gpuProp;

		VkSurfaceKHR surface;

		VulkanSwapchain* swapchain;

		VulkanQueue* graphicsQueue;
		VulkanQueue* presentQueue;

		VulkanRenderPass* renderPass;
	public:
		explicit VulkanDevice();
		virtual ~VulkanDevice();

		void Init(Window::Window* window, bool debug);
		virtual void Shutdown() override;

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		VkDevice GetRawDevice() const;
	private:
		void CreateSurface(Window::Window* window);
		void RequirePhysicalDevice();
		void CreateDevice(bool vaildationLayerOn);
		void CreateSwapchain(Window::Window* window);

		bool CheckDeviceFeatures(VkPhysicalDevice device);
	};
}