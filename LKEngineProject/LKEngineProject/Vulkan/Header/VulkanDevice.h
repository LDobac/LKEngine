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

	VulkanDescriptorSetLayout* descriptorSetLayout;
	VulkanDescriptorPool* descriptorPool;
	VulkanDescriptorSet* descriptorSet;
	VulkanGraphicsPipeline* graphicsPipeline;
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
	void InitInstance(bool debug);
	void InitSurface();
	void RequireGPU();
	void InitDevice(bool debug);
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