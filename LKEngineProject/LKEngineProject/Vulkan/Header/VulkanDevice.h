#pragma once

#include <vector>

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
	class VulkanSingleCommandPool;
	class VulkanGraphicsPipeline;
	class VulkanDescriptorSetLayout;
	class VulkanSemaphore;
	class VulkanDescriptorPool;
	class VulkanDescriptorSet;
	class VulkanMesh;
	
	class VulkanDevice 
	{
	private:
		LKEngine::Window::WindowsWindow* window;

		VulkanInstance* instance;

		VkDevice vkDevice;

		VkPhysicalDevice gpu;
		VkPhysicalDeviceProperties gpuProp;

		VkSurfaceKHR surface;

		VulkanSwapchain* swapchain;

		VulkanQueue* graphicsQueue;
		VulkanQueue* presentQueue;

		VulkanRenderPass* renderPass;

		VulkanDescriptorSetLayout* descriptorSetLayout;
		VulkanDescriptorPool* descriptorPool;
		VulkanDescriptorSet* descriptorSet;

		VulkanGraphicsPipeline* graphicsPipeline;

		VulkanCommandPool* commandPool;
		VulkanSingleCommandPool* singleCommandPool;

		VulkanSemaphore* imageAvailableSemaphore;
		VulkanSemaphore* renderFinishedSemaphore;

		QueueFamilyIndices queueIndices;

		VulkanMesh* mesh;
	public:
		explicit VulkanDevice(LKEngine::Window::WindowsWindow* window);
		virtual ~VulkanDevice();

		void Init(bool debug);
		virtual void Shutdown();

		void Update();
		void Draw();

		void ResizeWindow();

		void WaitIdle();

		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		QueueFamilyIndices GetQueueFamilyIndices() const;
		VkDevice GetHandle() const;
	private:
		void CreateSurface(LKEngine::Window::WindowsWindow * window);
		void RequirePhysicalDevice();
		void CreateDevice(bool vaildationLayerOn);
		void CreateQueue();
		void CreateDescriptorSetLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSet();
		void CreateCommandPool();
		void CreateSemaphore();

		void CreateDataBuffers();

		bool CheckDeviceFeatures(VkPhysicalDevice device);

		void RecordCommandBuffer();
	};
}