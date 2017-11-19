#include "../Header/VulkanDescriptorSet.h"

#include "../../Utility/Header/Macro.h"

#include "../Header/VulkanDescriptorSetLayout.h"
#include "../Header/VulkanDescriptorPool.h"
#include "../Header/VulkanBuffer.h"

using namespace LKEngine::Vulkan;

VulkanDescriptorSet::VulkanDescriptorSet(VulkanDevice * device)
	:VulkanDeviceChild(device)
{ }

void VulkanDescriptorSet::Init(VulkanDescriptorSetLayout* setLayout, VulkanDescriptorPool* pool)
{
	VkDescriptorSetLayout layouts[] = { setLayout->GetHandle() };
	VkDescriptorSetAllocateInfo allocInfo = { };
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool->GetHandle();
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = layouts;

	Check_Throw(vkAllocateDescriptorSets(device->GetHandle(), &allocInfo, &descriptorSet) != VK_SUCCESS, "디스크립터 셋 할당");
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
	descriptorWrite.pImageInfo = nullptr;
	descriptorWrite.pTexelBufferView = nullptr;
	
	descriptorWrites.push_back(descriptorWrite);
}

void VulkanDescriptorSet::UpdateSets()
{
	vkUpdateDescriptorSets(device->GetHandle(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

	bufferInfos.clear();
	descriptorWrites.clear();
}

const VkDescriptorSet & VulkanDescriptorSet::GetHandle() const
{
	return descriptorSet;
}
