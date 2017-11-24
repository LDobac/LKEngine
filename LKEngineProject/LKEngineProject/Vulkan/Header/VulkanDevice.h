#pragma once

#include <vector>

#include "VulkanInstance.h"
#include "VulkanQueueFamilyIndices.h"

#include "../../Utility/Header/Macro.h"

namespace LKEngine::Window
{
	class WindowsWindow;
}

LK_VULKAN_SPACE_BEGIN

class VulkanQueue;
class VulkanSwapchain;
class VulkanRenderPass;
class VulkanCommandPool;
class VulkanSingleCommandPool;
class VulkanSemaphore;

class VulkanDevice
{
private:
	static VulkanDevice* instance;

	explicit VulkanDevice();
	~VulkanDevice();
public:
	static VulkanDevice* GetInstance();
	static void Release();
private:
	bool isDebug;

	LKEngine::Window::WindowsWindow* window;

	VulkanInstance* vulkanInstance;

	VkPhysicalDevice gpu;
	VkPhysicalDeviceProperties gpuProp;

	VkDevice vkDevice;

	VkSurfaceKHR surface;

	VulkanSwapchain* swapchain;

	VulkanQueue* graphicsQueue;
	VulkanQueue* presentQueue;

	VulkanRenderPass* renderPass;

	VulkanCommandPool* commandPool;
	VulkanSingleCommandPool* singleCommandPool;

	VulkanSemaphore* imageAvailableSemaphore;
	VulkanSemaphore* renderFinishedSemaphore;

	QueueFamilyIndices queueIndices;
public:
	void SetDebugMode(bool debug);
	void SetWindow(LKEngine::Window::WindowsWindow* window);

	void Init();

	void Render();
	void Draw();

	void ResizeWindow();

	void WaitIdle();

	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	QueueFamilyIndices GetQueueFamilyIndices() const;
	VkDevice GetHandle() const;

	VulkanSwapchain* GetSwapchain() const;
	VulkanSingleCommandPool* GetSingleCommandPool() const;
	VulkanRenderPass* GetRenderPass() const;
private:
	void Shutdown();

	void InitInstance();
	void InitSurface();
	void RequireGPU();
	void InitDevice();
	void CreateQueue();
	void InitSwapchain();
	void InitCommandPool();
	void AllocCommandBuffers();
	void InitDepthBuffer();
	void InitRenderPass();
	void CreateFramebuffers();
	void CreateSemaphore();

	bool CheckDeviceFeatures(VkPhysicalDevice device);

};
LK_VULKAN_SPACE_END