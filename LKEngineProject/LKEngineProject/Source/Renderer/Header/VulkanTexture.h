#pragma once

#include "../Header/VulkanImage.h"
#include "../Header/VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanTexture
	: private VulkanImage
{
private:
	VkSampler sampler;
public:
	explicit VulkanTexture(std::string path);
	virtual ~VulkanTexture();

	const VkSampler& GetSampler() const;
	const VkImageView& GetImageView() const;
private:
	void CreateTexture(std::string path, VulkanSingleCommandPool* commandPool);
	void CreateSampler();
};

LK_VULKAN_SPACE_END