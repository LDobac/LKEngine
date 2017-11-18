#include "../Header/VulkanImage.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Vulkan;

VulkanImage::VulkanImage(VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(VK_NULL_HANDLE),
	imageView(VK_NULL_HANDLE)
{
}

VulkanImage::VulkanImage(VkImage & image, VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(image),
	imageView(VK_NULL_HANDLE)
{
}

 VulkanImage::~VulkanImage()
{ }

void VulkanImage::InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags)
{
	if (image == VK_NULL_HANDLE)
	{
		Console_Log("Warning : image가 초기화 되지 않았지만 InitWithoutImage를 호출");
		return;
	}

	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	//MEMO : 이미지의 타입을 지정 할 수 있음
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;

	//MEMO : 밉맵 및 다중 레이어 지정 가능
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkResult result = vkCreateImageView(device->GetHandle(), &viewInfo, nullptr, &imageView);
	Check_Throw(result != VK_SUCCESS, "이미지 뷰 생성 실패!");
}

 void VulkanImage::Shutdown()
{
	 vkDestroyImageView(device->GetHandle(), imageView, nullptr);
	 vkDestroyImage(device->GetHandle(), image, nullptr);
}

void VulkanImage::ShutdownWithoutImage()
{
	vkDestroyImageView(device->GetHandle(), imageView, nullptr);
}

void VulkanImage::CreateImage()
{
}

void VulkanImage::CreateImageView()
{
}
