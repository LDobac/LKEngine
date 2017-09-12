#include "../Header/VulkanDevice.h"

#include <vector>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/Window.h"
#include "../Header/VulkanQueueFamilyIndices.h"
#include "../Header/VulkanExtension.h"
#include "../Header/VulkanSwapchain.h"

using namespace LKEngine::Vulkan;

VulkanDevice::VulkanDevice()
	:instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE)
{
	instance = new VulkanInstance();
}

VulkanDevice::~VulkanDevice()
{
	SAFE_DELETE(instance);
}

void VulkanDevice::Init(LKEngine::Window::Window* window, bool debug)
{
	instance->Init(debug);

	CreateSurface(window);

	RequirePhysicalDevice();
}

void VulkanDevice::Shutdown()
{
	instance->Shutdown();
}

VkDevice VulkanDevice::GetRawDevice() const
{
	return vkDevice;
}

void VulkanDevice::RequirePhysicalDevice()
{
	Console_Log("그래픽 카드 선택");
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
	Console_Log("그래픽 카드 선택 완료!");

	vkGetPhysicalDeviceProperties(gpu, &gpuProp);
	auto GetDeviceTypeString = [&]()-> std::string {
		std::string str;
		switch (gpuProp.deviceType)
		{
		case  VK_PHYSICAL_DEVICE_TYPE_OTHER:
			str = "Other";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			str = "Integrated GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			str = "Discrete GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			str = "Virtual GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			str = "CPU";
			break;
		default:
			str = "Unknown";
			break;
		}
		return str;
	};
	
	Console_Log("그래픽 카드 정보 : ");
	Console_Log_Format("API : 0x%x , Driver : 0x%x , VecdorID : 0x%x", gpuProp.apiVersion, gpuProp.driverVersion, gpuProp.vendorID);
	Console_Log_Format("Name \"%s\" , DeviceID : 0x%x , Type \"%s\"", gpuProp.deviceName, gpuProp.deviceID, GetDeviceTypeString().c_str());
	Console_Log_Format("Max Descriptor Sets Bound : %d , Timestamps : %d", gpuProp.limits.maxBoundDescriptorSets, gpuProp.limits.timestampComputeAndGraphics);
}

bool VulkanDevice::CheckDeviceFeatures(VkPhysicalDevice device)
{
	QueueFamilyIndices queueFamilyIndices;
	VulkanExtension extension;
	SwapchainSupportDetail supportDetail(device, surface);

	if (queueFamilyIndices.FindQueueFamily(device,surface))
	{
		bool deviceExtensionSupport = extension.CheckDeviceExtensionSupport(device);
		bool swapSupport = supportDetail.CheckSwapchainAdequate();

		//MEMO : 디바이스 기능을 더 확인 할 수 있음
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		bool deviceFeatureSupport = supportedFeatures.samplerAnisotropy;

		return deviceExtensionSupport && deviceFeatureSupport && swapSupport;
	}
	
	return false;
}

void VulkanDevice::CreateSurface(LKEngine::Window::Window* window)
{
	VkResult result = glfwCreateWindowSurface(instance->GetRawInstance(), window->GetWindowHandle(), nullptr, &surface);
	Check_Throw(result != VK_SUCCESS, "Surface가 생성되지 않았습니다!");
}
