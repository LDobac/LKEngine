#include "VulkanDevice.h"

#include <vector>

#include "Macro.h"

LKEngine::Vulkan::VulkanDevice::VulkanDevice()
	:instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE)
{
	instance = new VulkanInstance(true);
}

LKEngine::Vulkan::VulkanDevice::~VulkanDevice()
{
	delete instance;
	instance = nullptr;
}

VkDevice LKEngine::Vulkan::VulkanDevice::GetRawDevice() const
{
	return vkDevice;
}

void LKEngine::Vulkan::VulkanDevice::RequirePhysicalDevice()
{
	gpu = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetRawInstance(), &deviceCount, devices.data());

	//TODO : 차후에 사용 가능한 GPU가 다수 일경우 그중에서 가장 적합한 GPU를 선택하도록 변경
	for (auto device : devices)
	{
		if (CheckDeviceFeatures(device))
		{
			gpu = device;
			break;
		}
	}
	Check_Throw(gpu == VK_NULL_HANDLE, "사용 가능한 그래픽 카드가 없습니다!");
}

bool LKEngine::Vulkan::VulkanDevice::CheckDeviceFeatures(VkPhysicalDevice device)
{
	return false;
}
