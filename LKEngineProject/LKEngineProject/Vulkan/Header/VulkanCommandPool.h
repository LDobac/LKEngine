#pragma once

#include "VulkanDeviceChild.h"

LK_VULKAN_SPACE_BEGIN

class VulkanCommandBuffers;

class VulkanCommandPool
	: public VulkanDeviceChild
{
private:
	VkCommandPool commandPool = VK_NULL_HANDLE;

	VulkanCommandBuffers* commandBuffers;
public:
	VulkanCommandPool(VulkanDevice* device);
	virtual ~VulkanCommandPool();

	void Init(VkCommandPoolCreateFlags flags,int32_t queueIndex);
	virtual void Shutdown();

	void AllocBuffers(size_t size);
	void FreeBuffers();

	void RecordBegin(uint32_t index, VkCommandBufferUsageFlags flags);
	void RecordEnd(uint32_t index);

	const VkCommandBuffer& GetBuffer(uint32_t index) const;
	size_t GetBufferSize() const;
	const VkCommandPool& GetHandle() const;
};

class VulkanSingleCommandPool
	: public VulkanDeviceChild
{
private:
	VkCommandPool commandPool = VK_NULL_HANDLE;

	VulkanCommandBuffers* commandBuffers;

	VulkanQueue* transferQueue;
public:
	explicit VulkanSingleCommandPool(VulkanDevice* device);
	~VulkanSingleCommandPool();

	void Init(VulkanQueue* queue);
	void Shutdown();

	const VkCommandBuffer& RecordBegin();
	void RecordEnd();

	const VkCommandPool& GetHandle() const;
};

LK_VULKAN_SPACE_END