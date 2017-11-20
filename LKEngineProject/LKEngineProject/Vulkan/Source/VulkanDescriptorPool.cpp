#include "../Header/VulkanDescriptorPool.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanDescriptorPool::VulkanDescriptorPool(VulkanDevice * device)
	:VulkanDeviceChild(device)
{
}

void LKEngine::Vulkan::VulkanDescriptorPool::AddPoolSize(VkDescriptorType type, size_t descriptorCount)
{
	VkDescriptorPoolSize poolSize = { };
	poolSize.type = type;
	poolSize.descriptorCount = descriptorCount;
	poolSizes.push_back(poolSize);
}

void VulkanDescriptorPool::CreatePool(size_t maxSets)
{
	VkDescriptorPoolCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.poolSizeCount = poolSizes.size();
	info.pPoolSizes = poolSizes.data();
	info.maxSets = maxSets;

	Check_Throw(vkCreateDescriptorPool(device->GetHandle(), &info, nullptr, &descriptorPool) != VK_SUCCESS, "디스크립터 풀 생성 실패");
}

void VulkanDescriptorPool::Shutdown()
{
	vkDestroyDescriptorPool(device->GetHandle(), descriptorPool, nullptr);
}

const VkDescriptorPool & VulkanDescriptorPool::GetHandle() const
{
	return descriptorPool;
}
