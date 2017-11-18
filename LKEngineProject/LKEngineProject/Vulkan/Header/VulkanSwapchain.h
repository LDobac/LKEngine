#pragma once

#include <vulkan/vulkan.hpp>

#include "IVulkanObject.h"
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
		: public IVulkanObject, VulkanDeviceChild
	{
	private:
		VkSwapchainKHR swapchain;

		VkFormat swapchainFormat;
		VkExtent2D swapchainExtent;

		std::vector<VulkanImage*> swapchainImages;

		Window::WindowsWindow* window;
	public:
		explicit VulkanSwapchain(VulkanDevice* device, Window::WindowsWindow* window);
		virtual ~VulkanSwapchain();

		virtual void Init() override { }
		void Init(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface, QueueFamilyIndices& queueIndices);
		virtual void Shutdown() override;

		VkFormat GetFormat() const;
		const std::vector<VulkanImage*>& GetImages() const;
	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const;
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	};
}