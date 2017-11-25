#pragma once

#include <vulkan/vulkan.h>

#include "../../Utility/Header/Macro.h"

LK_VULKAN_SPACE_BEGIN

class VulkanExtension;
class VulkanDebug;

class VulkanInstance
{
private:
	VkInstance vkInstance;

	VulkanDebug* debug;
public :
	explicit VulkanInstance();
	~VulkanInstance();

	void Init(bool vaildationLayerOn);
	virtual void Shutdown();

	VkInstance GetHandle() const;
};

LK_VULKAN_SPACE_END