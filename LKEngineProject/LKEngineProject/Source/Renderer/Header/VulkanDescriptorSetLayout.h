#pragma once

#include <vulkan/vulkan.hpp>
#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class DescriptorSetLayout : public TSingleton<DescriptorSetLayout>
{
public:
	static VulkanDescriptorSetLayout* cameraLayout;
	static VulkanDescriptorSetLayout* meshLayout;
	static VulkanDescriptorSetLayout* materialLayout;

	DescriptorSetLayout()
	{
		cameraLayout = new VulkanDescriptorSetLayout();
		cameraLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		cameraLayout->CreateDescriptorSetLayout();

		meshLayout = new VulkanDescriptorSetLayout();
		meshLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
		meshLayout->CreateDescriptorSetLayout();

		materialLayout = new VulkanDescriptorSetLayout();
		materialLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
		materialLayout->CreateDescriptorSetLayout();
	}

	~DescriptorSetLayout()
	{
		SAFE_DELETE(cameraLayout);
		SAFE_DELETE(meshLayout);
		SAFE_DELETE(materialLayout);
	}
};

class VulkanDescriptorSetLayout
	: public VulkanDeviceChild
{
private:
	VkDescriptorSetLayout setLayout;
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
public:
	explicit VulkanDescriptorSetLayout();
	~VulkanDescriptorSetLayout();

	void AddDescriptor(VkDescriptorType type, VkShaderStageFlagBits stageFlag, int index);
	void CreateDescriptorSetLayout();

	const VkDescriptorSetLayout& GetHandle() const;
};

LK_VULKAN_SPACE_END