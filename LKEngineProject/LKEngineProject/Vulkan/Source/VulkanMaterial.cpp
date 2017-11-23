#include "../Header/VulkanMaterial.h"

#include "../Header/VulkanDescriptorSet.h"
#include "../Header/VulkanTexture.h"
#include "../../Src/EntityPool.h"

USING_LK_VULKAN_SPACE

VulkanMaterial::VulkanMaterial(VulkanDescriptorSetLayout* setLayout, VulkanPipeline* pipeline)
	:pipeline(pipeline)
{
	CreateDescriptorSet(setLayout);
}

VulkanMaterial::~VulkanMaterial()
{
	SAFE_DELETE(descriptorSet);
	for (size_t i = 0; i < textures.size(); i++)
	{
		SAFE_DELETE(textures[i]);
	}
}

void VulkanMaterial::AddTexture(VulkanTexture * texture)
{
	descriptorSet->AddTextureInfo(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, texture, textures.size());

	descriptorSet->UpdateSets();

	textures.push_back(texture);
}

void VulkanMaterial::SetVulkanPipeline(VulkanPipeline * pipeline)
{
	this->pipeline = pipeline;
}

std::vector<VulkanTexture*> VulkanMaterial::GetTexture() const
{
	return textures;
}

VulkanDescriptorSet * VulkanMaterial::GetDescriptorSet() const
{
	return descriptorSet;
}

VulkanPipeline * VulkanMaterial::GetPipeline() const
{
	return pipeline;
}

void VulkanMaterial::CreateDescriptorSet(VulkanDescriptorSetLayout* setLayout)
{
	descriptorSet = new Vulkan::VulkanDescriptorSet(setLayout, EntityPool::GetInstance()->GetDescriptorPool());
	descriptorSet->AddTextureInfo(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textures[0], 0);
	descriptorSet->UpdateSets();
}
