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

 void VulkanImage::Init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
 {
	 CreateImage(width, height, format, tiling, usageFlag, properties);
	 CreateMemory(properties);
	 CreateImageView(format, aspectFlags);
 }

 void VulkanImage::InitWithoutImage(VkFormat format, VkImageAspectFlags aspectFlags)
{
	if (image == VK_NULL_HANDLE)
	{
		Console_Log("Warning : image가 초기화 되지 않았지만 InitWithoutImage를 호출");
		return;
	}

	CreateImageView(format, aspectFlags);
}

 void VulkanImage::Shutdown()
{
	 vkDestroyImageView(device->GetHandle(), imageView, nullptr);
	 vkFreeMemory(device->GetHandle(), memory, nullptr);
	 vkDestroyImage(device->GetHandle(), image, nullptr);
}

void VulkanImage::ShutdownWithoutImage()
{
	vkDestroyImageView(device->GetHandle(), imageView, nullptr);
}

VkImage VulkanImage::GetImage() const
{
	return image;
}

const VkImageView& VulkanImage::GetImageView() const
{
	return imageView;
}

void VulkanImage::CreateImage(uint32_t width,uint32_t height, VkFormat format, VkImageTiling tiling,VkImageUsageFlags usageFlag,VkMemoryPropertyFlags properties)
{
	VkImageCreateInfo imageInfo = { };
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usageFlag;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	Check_Throw(vkCreateImage(device->GetHandle(), &imageInfo, nullptr, &image),"이미지 생성 실패");
}

void VulkanImage::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags)
{
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

void VulkanImage::CreateMemory(VkMemoryPropertyFlags properties)
{
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device->GetHandle(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, properties);

	Check_Throw(vkAllocateMemory(device->GetHandle(), &allocInfo, nullptr, &memory) != VK_SUCCESS, "ImageMemory 생성 실패");

	vkBindImageMemory(device->GetHandle(), image, memory, 0);
}
