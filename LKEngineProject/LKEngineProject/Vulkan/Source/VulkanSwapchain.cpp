#include "../Header/VulkanSwapchain.h"

#include <GLFW/glfw3.h>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/WindowsWindow.h"
#include "../Header/VulkanDevice.h"
#include "../Header/VulkanImage.h"
#include "../Header/VulkanRenderPass.h"

using namespace LKEngine::Vulkan;

SwapchainSupportDetail::SwapchainSupportDetail(VkPhysicalDevice gpu, VkSurfaceKHR surface)
{
	//서페이스 기능 질의
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &capabilities);

	//서페이스 포맷 질의
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, nullptr);
	if (formatCount != 0) 
	{
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &formatCount, formats.data());
	}

	//서페이스 프레젠트 모드 질의
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) 
	{
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu, surface, &presentModeCount, presentModes.data());
	}
}

bool SwapchainSupportDetail::CheckSwapchainAdequate()
{
	return !(formats.empty() || presentModes.empty());
}

VulkanSwapchain::VulkanSwapchain(VulkanDevice* device, LKEngine::Window::WindowsWindow* window)
	: VulkanDeviceChild(device),
	window(window),
	swapchain(VK_NULL_HANDLE)
{
	depthImage = new VulkanImage(device);
}

VulkanSwapchain::~VulkanSwapchain()
{
	for (auto& image : swapchainImages)
	{
		SAFE_DELETE(image);
	}
	SAFE_DELETE(depthImage);
}

void VulkanSwapchain::Init(const VkPhysicalDevice& gpu, const VkSurfaceKHR& surface, QueueFamilyIndices& queueIndices , VulkanSwapchain* oldSwapchain)
{
	Console_Log("스왑 체인 생성 시작");

	SwapchainSupportDetail swapchainSupport(gpu, surface);

	auto surfaceFormat = ChooseSurfaceFormat(swapchainSupport.formats);
	auto presentMode = ChoosePresentMode(swapchainSupport.presentModes);
	auto extent = ChooseSwapchainExtent(swapchainSupport.capabilities);

	uint32_t imageCount = swapchainSupport.capabilities.minImageCount + 1;
	if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
	{
		imageCount = swapchainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	if(oldSwapchain != nullptr) createInfo.oldSwapchain = oldSwapchain->GetHandle();
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	queueIndices.FindQueueFamily(gpu, surface);
	uint32_t queueFamilyIndices[] = { static_cast<uint32_t>(queueIndices.graphicsFamily),static_cast<uint32_t>(queueIndices.presentFamily) };

	if (queueIndices.graphicsFamily != queueIndices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	//MEMO : 스왑 체인 이미지에 변환 적용 가능
	createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
	//MEMO : Windows에서 다른 윈도우와 혼합하기 위해 알파 채널을 사용하는지 여부 설정 가능
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	VkResult result = vkCreateSwapchainKHR(device->GetHandle(), &createInfo, nullptr, &swapchain);
	Check_Throw(result != VK_SUCCESS, "스왑 체인 생성 실패!");

	std::vector<VkImage> tmpImages;
	vkGetSwapchainImagesKHR(device->GetHandle(), swapchain, &imageCount, nullptr);
	tmpImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device->GetHandle(), swapchain, &imageCount, tmpImages.data());

	swapchainImages.resize(imageCount);
	frameBuffers.resize(imageCount);
	for (size_t i = 0; i < swapchainImages.size(); i++)
	{
		VulkanImage* image = new VulkanImage(tmpImages[i], device);
		image->InitWithoutImage(surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);

		swapchainImages[i] = image;
	}

	swapchainFormat = surfaceFormat.format;
	swapchainExtent = extent;

	Console_Log("스왑 체인 생성 성공");
}

void VulkanSwapchain::Shutdown()
{
	for (size_t i = 0; i < swapchainImages.size(); i++)
	{
		swapchainImages[i]->ShutdownWithoutImage();
		vkDestroyFramebuffer(device->GetHandle(), frameBuffers[i], nullptr);
	}

	vkDestroySwapchainKHR(device->GetHandle(), swapchain, nullptr);
	depthImage->Shutdown();
}

void VulkanSwapchain::InitDepthBuffer(VulkanSingleCommandPool * commandPool)
{
	Console_Log("깊이 버퍼 생성 시작");
	VkFormat depthFormat = device->FindSupportedFormat({
		VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT
	},
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	depthImage->Init(swapchainExtent.width, swapchainExtent.height, 
		depthFormat, 
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		VK_IMAGE_ASPECT_DEPTH_BIT);
	depthImage->TransitionLayout(commandPool, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	Console_Log("깊이 버퍼 생성 성공");
}

void VulkanSwapchain::CreateFrameBuffers(VulkanRenderPass * renderPass)
{
	Console_Log("프레임버퍼 생성 시작");
	frameBuffers.resize(swapchainImages.size());
	for (size_t i = 0; i < frameBuffers.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
			swapchainImages[i]->GetImageView(),
			depthImage->GetImageView()
		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->GetHandle();
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapchainExtent.width;
		framebufferInfo.height = swapchainExtent.height;
		framebufferInfo.layers = 1;

		Check_Throw(vkCreateFramebuffer(device->GetHandle(), &framebufferInfo, nullptr, &frameBuffers[i]), "프레임 버퍼 생성 실패");
	}
	Console_Log("프레임버퍼 생성 성공");
}

const VkSwapchainKHR & VulkanSwapchain::GetHandle() const
{
	return swapchain;
}

VkFormat VulkanSwapchain::GetFormat() const
{
	return swapchainFormat;
}

VkExtent2D VulkanSwapchain::GetExtent() const
{
	return swapchainExtent;
}

const std::vector<VulkanImage*>& VulkanSwapchain::GetImages() const
{
	return swapchainImages;
}

const std::vector<VkFramebuffer> VulkanSwapchain::GetFrameBuffers() const
{
	return frameBuffers;
}

VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
{
	//Best One
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
	{
		return VkSurfaceFormatKHR{ VK_FORMAT_B8G8R8A8_UNORM,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	//Others
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	//Worst One
	return availableFormats[0];
}

VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) const
{
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentModes : availablePresentModes)
	{
		if (availablePresentModes == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentModes;
		}
		else if (availablePresentModes == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			bestMode = availablePresentModes;
		}
	}

	return bestMode;
}

VkExtent2D VulkanSwapchain::ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR & capabilities) const
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetWindowSize(window->GetWindowHandle(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}
