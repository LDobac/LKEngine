#include "../Header/VulkanDevice.h"

#include <vector>
#include <set>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/WindowsWindow.h"
#include "../Header/VulkanQueue.h"
#include "../Header/VulkanQueueFamilyIndices.h"
#include "../Header/VulkanExtension.h"
#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanRenderPass.h"

using namespace LKEngine::Vulkan;

VulkanDevice::VulkanDevice()
	:instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE),
	surface(VK_NULL_HANDLE),
	swapchain(nullptr),
	graphicsQueue(nullptr),
	presentQueue(nullptr),
	renderPass(nullptr)
{
	instance = new VulkanInstance();

	renderPass = new VulkanRenderPass(this);
}

VulkanDevice::~VulkanDevice()
{
	SAFE_DELETE(instance);
	SAFE_DELETE(swapchain);
	SAFE_DELETE(renderPass);
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
}

void VulkanDevice::Init(LKEngine::Window::WindowsWindow* window, bool debug)
{
	instance->Init(debug);

	CreateSurface(window);

	RequirePhysicalDevice();

	CreateDevice(debug);

	CreateSwapchain(window);

	renderPass->Init(swapchain);
}

void VulkanDevice::Shutdown()
{
	renderPass->Shutdown();

	swapchain->Shutdown();

	vkDestroySurfaceKHR(*(*instance), surface, nullptr);

	instance->Shutdown();
}

VkFormat VulkanDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) 
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(gpu, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
		{
			return format;
		}
	}

	Check_Throw(true, "지원되는 포맷을 찾을 수 없습니다!");
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
	vkEnumeratePhysicalDevices(*(*instance), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(*(*instance), &deviceCount, devices.data());

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

void VulkanDevice::CreateDevice(bool vaildationLayerOn)
{
	Console_Log("디바이스 생성 시작");

	QueueFamilyIndices indices;
	indices.FindQueueFamily(gpu, surface);

	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily,indices.presentFamily };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//TODO : 나중에 큐가 여러개 있을경우 처리 및 큐의 우선 순의 설정
	float queuePriority = 1.0f;
	for (size_t i = 0; i < uniqueQueueFamilies.size(); i++)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = i;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		queueCreateInfos.push_back(queueCreateInfo);
	}

	//TODO : 사용할 GPU의 기능을 지정한다.
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;


	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Vulkan::deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = Vulkan::deviceExtensions.data();

	if (vaildationLayerOn)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::vaildationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = Vulkan::vaildationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &vkDevice);
	Check_Throw(result != VK_SUCCESS, "디바이스 생성 실패!");

	graphicsQueue = new VulkanQueue(this, indices.graphicsFamily, 0);
	presentQueue = new VulkanQueue(this, indices.presentFamily, 0);

	Console_Log("디바이스 생성 성공");
}

void VulkanDevice::CreateSwapchain(LKEngine::Window::WindowsWindow * window)
{
	swapchain = new VulkanSwapchain(this, window);
	swapchain->Init(gpu, surface);
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

		//MEMO : 차후에 GPU가 어떤 기능을 사용해야 하는지에 대해 확인 할 수 있음
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		bool deviceFeatureSupport = supportedFeatures.samplerAnisotropy;

		return deviceExtensionSupport && deviceFeatureSupport && swapSupport;
	}
	
	return false;
}

void VulkanDevice::CreateSurface(LKEngine::Window::WindowsWindow * window)
{
	VkResult result = glfwCreateWindowSurface(*(*instance), window->GetWindowHandle(), nullptr, &surface);
	Check_Throw(result != VK_SUCCESS, "Surface가 생성되지 않았습니다!");
}
