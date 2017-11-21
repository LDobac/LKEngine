#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanTexture;
class VulkanDescriptorSet;
class VulkanDescriptorSetLayout;
class VulkanPipeline;

class VulkanMaterial
{
private:
	std::vector<VulkanTexture*> textures;
	VulkanDescriptorSet* descriptorSet;
	VulkanPipeline* pipeline;
public:
	explicit VulkanMaterial(VulkanDescriptorSetLayout* setLayout, VulkanPipeline* pipeline);
	~VulkanMaterial();

	void AddTexture(VulkanTexture* texture);

	void SetVulkanPipeline(VulkanPipeline * pipeline);

	std::vector<VulkanTexture*> GetTexture() const;
	VulkanDescriptorSet* GetDescriptorSet() const;
	VulkanPipeline* GetPipeline() const;
};

LK_VULKAN_SPACE_END