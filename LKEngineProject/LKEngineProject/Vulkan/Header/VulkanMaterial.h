#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanTexture;
class VulkanDescriptorSet;
class VulkanPipeline;

class VulkanMaterial
{
private:
	//TODO : Multiple Textuer
	VulkanTexture* texture;
	VulkanDescriptorSet* descriptorSet;
	VulkanPipeline* pipeline;
public:

	VulkanTexture* GetTexture() const;
	VulkanDescriptorSet* GetDescriptorSet() const;
	VulkanPipeline* GetPipeline() const;
};

LK_VULKAN_SPACE_END