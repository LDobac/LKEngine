#pragma once

#include "../Header/VulkanImage.h"
#include "../Header/VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanTexture
	: public VulkanImage
{
private:
	VkSampler sampler;
public:
	explicit VulkanTexture(VulkanDevice* device);

	void Init(std::string path,VulkanSingleCommandPool* commandPool);
	virtual void Shutdown() override;

	const VkSampler& GetSampler() const;
private:
	void CreateTexture(std::string path, VulkanSingleCommandPool* commandPool);
	void CreateSampler();
};

LK_VULKAN_SPACE_END