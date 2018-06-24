#include "../Header/Material.h"

#include "../../Renderer/Header/VulkanDescriptorSet.h"

#include "../Header/Texture.h"
#include "../Header/EntityPool.h"

USING_LK_SPACE

Material::Material(Vulkan::VulkanPipeline * pipeline)
	: pipeline(pipeline)
{
	descriptorSet = new Vulkan::VulkanDescriptorSet(setLayout, EntityPool::GetInstance()->GetDescriptorPool());
}

Material::~Material()
{
	SAFE_DELETE(descriptorSet);
}

void Material::AddTexture(Texture * texture)
{
	descriptorSet->AddTextureInfo(
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
		texture->GetImageView(),
		texture->GetSampler() ,
		static_cast<uint32_t>(textures.size()));

	descriptorSet->UpdateSets();

	textures.push_back(texture);
}

Vulkan::VulkanDescriptorSet * Material::GetDescriptorSet() const
{
	return descriptorSet;
}

Vulkan::VulkanPipeline * Material::GetPipeline() const
{
	return pipeline;
}
