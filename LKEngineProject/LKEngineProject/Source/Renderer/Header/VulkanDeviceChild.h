#pragma once

#include <vulkan/vulkan.hpp>

#include "VulkanDevice.h"

#include "../../Utility/Header/Macro.h"

LK_VULKAN_SPACE_BEGIN

class VulkanDeviceChild
{
protected:
	VulkanDevice* device;
public:
	explicit VulkanDeviceChild(VulkanDevice* device) :device(device) {}
	virtual ~VulkanDeviceChild() {}
};

LK_VULKAN_SPACE_END