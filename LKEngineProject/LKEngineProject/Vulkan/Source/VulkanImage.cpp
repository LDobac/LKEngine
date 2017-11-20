#include "../Header/VulkanImage.h"

#include "../Header/VulkanCommandPool.h"
#include "../Header/VulkanBuffer.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanImage::VulkanImage(VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(VK_NULL_HANDLE),
	imageView(VK_NULL_HANDLE)
{ }

VulkanImage::VulkanImage(VkImage & image, VulkanDevice * device)
	: VulkanDeviceChild(device),
	image(image),
	imageView(VK_NULL_HANDLE)
{ }

 VulkanImage::~VulkanImage()
{ }

 void VulkanImage::Init(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlag, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
 {
	 CreateImage(width, height, format, tiling, usageFlag, properties);
	 CreateMemory(properties);
	 CreateImageView(format, aspectFlags);
	 this->width = width;
	 this->height = height;
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

size_t VulkanImage::GetWidth() const
{
	return width;
}

size_t VulkanImage::GetHeight() const
{
	return height;
}

VkImage VulkanImage::GetImage() const
{
	return image;
}

VkFormat VulkanImage::GetFormat() const
{
	return format;
}

const VkImageView& VulkanImage::GetImageView() const
{
	return imageView;
}

void VulkanImage::CreateImage(uint32_t width,uint32_t height, VkFormat format, VkImageTiling tiling,VkImageUsageFlags usageFlag,VkMemoryPropertyFlags properties)
{
	this->format = format;
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
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
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

void VulkanImage::TransitionLayout(VulkanSingleCommandPool * commandPool, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = commandPool->RecordBegin();

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;

	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

	barrier.image = image;
	if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		auto HasStencilComponent = [](VkFormat format) {
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		};
		if (HasStencilComponent(format))
		{
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
	}
	else
	{
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	}
	
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags srcStage;
	VkPipelineStageFlags dstStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) 
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	}
	else 
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		srcStage, dstStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	commandPool->RecordEnd();
}

void VulkanImage::CopyBufferToImage(VulkanBuffer * srcBuffer, VulkanImage * dstImagem, VulkanSingleCommandPool * commandPool)
{
	VkCommandBuffer commandBuffer = commandPool->RecordBegin();

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		width,
		height,
		1 };

	vkCmdCopyBufferToImage(
		commandBuffer,
		srcBuffer->GetBuffer(),
		image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	commandPool->RecordEnd();
}
