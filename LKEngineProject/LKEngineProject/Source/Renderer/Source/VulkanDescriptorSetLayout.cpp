#include "../Header/VulkanDescriptorSetLayout.h"

#include "../../Utility/Header/Macro.h"

USING_LK_VULKAN_SPACE

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout()
	: VulkanDeviceChild(VulkanDevice::GetInstance())
{
}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(device->GetHandle(), setLayout, nullptr);
}

void VulkanDescriptorSetLayout::AddDescriptor(VkDescriptorType type, VkShaderStageFlagBits stageFlag, int index)
{
	VkDescriptorSetLayoutBinding layoutBinding = { };
	layoutBinding.binding = index;
	layoutBinding.descriptorCount = 1;
	layoutBinding.descriptorType = type;
	layoutBinding.pImmutableSamplers = nullptr;
	layoutBinding.stageFlags = stageFlag;
	layoutBindings.push_back(layoutBinding);
}

void VulkanDescriptorSetLayout::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutCreateInfo layoutInfo = { };
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutInfo.pBindings = layoutBindings.data();

	Check_Throw(vkCreateDescriptorSetLayout(device->GetHandle(), &layoutInfo, nullptr, &setLayout), " DescriptorSetLayout 생성 실패");
}

const VkDescriptorSetLayout& VulkanDescriptorSetLayout::GetHandle() const
{
	return setLayout;
}
