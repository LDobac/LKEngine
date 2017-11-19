#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanDeviceChild.h"

//Foward Declaration
namespace LKEngine::Window
{
	class WindowsWindow;
}

namespace LKEngine::Vulkan
{
	class VulkanDevice;
	class VulkanImage;
	class VulkanRenderPass;

	struct SwapchainSupportDetail
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

	public:
		explicit SwapchainSupportDetail(VkPhysicalDevice gpu,VkSurfaceKHR surface);
		bool CheckSwapchainAdequate();
	};

	class VulkanSwapchain
		: public VulkanDeviceChild
	{
	private:
		VkSwapchainKHR swapchain;

		VkFormat swapchainFormat;
		VkExtent2D swapchainExtent;

		std::vector<VulkanImage*> swapchainImages;
		std::vector<VkFramebuffer> frameBuffers;

		VulkanImage* depthImage;

		Window::WindowsWindow* window;
	public:
		explicit VulkanSwapchain(VulkanDevice* device, Window::WindowsWindow* window);
		virtual ~VulkanSwapchain();

		void Init(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface, QueueFamilyIndices& queueIndices,VulkanSwapchain* oldSwapchain = nullptr);
		virtual void Shutdown();

		void InitDepthBuffer(VulkanSingleCommandPool* commandPool);

		void CreateFrameBuffers(VulkanRenderPass* renderPass);

		const VkSwapchainKHR& GetHandle() const;
		VkFormat GetFormat() const;
		VkExtent2D GetExtent() const;
		const std::vector<VulkanImage*>& GetImages() const;
		const std::vector<VkFramebuffer> GetFrameBuffers() const;
	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const;
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	};
}