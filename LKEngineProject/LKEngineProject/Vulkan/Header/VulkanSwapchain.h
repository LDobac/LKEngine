#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanBaseInterface.h"
#include "VulkanDeviceChild.h"

//Foward Declaration
namespace LKEngine::Window
{
	class Window;
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
		: public VulkanBaseInterface, VulkanDeviceChild
	{
	private:
		VkSwapchainKHR swapchain;

		VkFormat swapchainFormat;
		VkExtent2D swapchainExtent;

		std::vector<VulkanImage*> swapchainImages;

		Window::Window* window;
	public:
		explicit VulkanSwapchain(VulkanDevice* device, Window::Window* window);
		virtual ~VulkanSwapchain();

		void Init(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface);
		virtual void Shutdown() override;

		VkFormat GetFormat() const;
	private:
		VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const;
		VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const;
		VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	};
}