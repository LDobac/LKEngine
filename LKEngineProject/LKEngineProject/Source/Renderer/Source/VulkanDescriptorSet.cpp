#include "../Header/VulkanDescriptorSet.h"

#include "../Header/VulkanDescriptorSetLayout.h"
#include "../Header/VulkanDescriptorPool.h"
#include "../Header/VulkanBuffer.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanDescriptorSet::VulkanDescriptorSet(VulkanDescriptorSetLayout* setLayout, VulkanDescriptorPool* pool)
	:VulkanDeviceChild(VulkanDevice::GetInstance())
{
	VkDescriptorSetLayout layouts[] = { setLayout->GetHandle() };
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool->GetHandle();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	Check_Throw(vkAllocateDescriptorSets(device->GetHandle(), &allocInfo, &descriptorSet) != VK_SUCCESS, "��ũ���� �� �Ҵ� ����");
}

void VulkanDescriptorSet::AddBufferInfo(VkDescriptorType type, VulkanBuffer * buffer, size_t offset, uint32_t binding)
{
	VkDescriptorBufferInfo info;
	info.buffer = buffer->GetBuffer();
	info.offset = offset;
	info.range = buffer->GetBufferSize();
	bufferInfos.push_back(info);

	VkWriteDescriptorSet descriptorWrite = { };
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = type;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfos.back();
	
	descriptorWrites.push_back(descriptorWrite);
}

void VulkanDescriptorSet::AddTextureInfo(VkDescriptorType type, const VkImageView& imageView, const VkSampler& sampler, uint32_t binding)
{
	VkDescriptorImageInfo info = { };
	info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	info.imageView = imageView;
	info.sampler = sampler;
	imageInfos.push_back(info);

	VkWriteDescriptorSet descriptorWrite = {};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = descriptorSet;
	descriptorWrite.dstBinding = binding;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = type;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfos.back();

	descriptorWrites.push_back(descriptorWrite);
}

void VulkanDescriptorSet::UpdateSets()
{
	vkUpdateDescriptorSets(device->GetHandle(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

const VkDescriptorSet & VulkanDescriptorSet::GetHandle() const
{
	return descriptorSet;
}
