#pragma once

#include <vector>

#include "IVulkanObject.h"
#include "VulkanInstance.h"
#include "VulkanQueueFamilyIndices.h"

//Foward Declaration
namespace LKEngine::Window
{
	class WindowsWindow;
}

namespace LKEngine::Vulkan
{
	class VulkanQueue;
	class VulkanSwapchain;
	class VulkanRenderPass;
	class VulkanCommandPool;

	class VulkanDevice 
		: public IVulkanObject
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

		VulkanCommandPool* commandPool;

		QueueFamilyIndices queueIndices;
	public:
		explicit VulkanDevice();
		virtual ~VulkanDevice();

		virtual void Init() override { }
		void Init(LKEngine::Window::WindowsWindow* window, bool debug);
		virtual void Shutdown() override;

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		QueueFamilyIndices GetQueueFamilyIndices() const;
		VkDevice GetHandle() const;
	private:
		void CreateSurface(LKEngine::Window::WindowsWindow * window);
		void RequirePhysicalDevice();
		void CreateDevice(bool vaildationLayerOn);
		void CreateQueue();
		void CreateSwapchain(LKEngine::Window::WindowsWindow * window);
		void CreateCommandPool();

		bool CheckDeviceFeatures(VkPhysicalDevice device);
	};
}