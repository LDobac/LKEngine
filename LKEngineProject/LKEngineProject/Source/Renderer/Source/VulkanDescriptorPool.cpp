#include "../Header/VulkanDescriptorPool.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanDescriptorPool::VulkanDescriptorPool()
	:VulkanDeviceChild(VulkanDevice::GetInstance())
{
}

VulkanDescriptorPool::~VulkanDescriptorPool()
{
	vkDestroyDescriptorPool(device->GetHandle(), descriptorPool, nullptr);
}

void LKEngine::Vulkan::VulkanDescriptorPool::AddPoolSize(VkDescriptorType type, size_t descriptorCount)
{
	VkDescriptorPoolSize poolSize = { };
	poolSize.type = type;
	poolSize.descriptorCount = static_cast<uint32_t>(descriptorCount);
	poolSizes.push_back(poolSize);
}

void VulkanDescriptorPool::CreatePool(size_t maxSets)
{
	VkDescriptorPoolCreateInfo info = { };
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	info.pPoolSizes = poolSizes.data();
	info.maxSets = static_cast<uint32_t>(maxSets);

	Check_Throw(vkCreateDescriptorPool(device->GetHandle(), &info, nullptr, &descriptorPool) != VK_SUCCESS, "��ũ���� Ǯ ���� ����");
}

const VkDescriptorPool & VulkanDescriptorPool::GetHandle() const
{
	return descriptorPool;
}
