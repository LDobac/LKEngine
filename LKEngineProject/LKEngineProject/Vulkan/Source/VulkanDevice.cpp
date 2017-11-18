#include "../Header/VulkanDevice.h"

#include <vector>
#include <set>
#include <numeric>

#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/WindowsWindow.h"
#include "../Header/VulkanQueue.h"
#include "../Header/VulkanExtension.h"
#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanRenderPass.h"
#include "../Header/VulkanCommandPool.h"
#include "../Header/VulkanPipeline.h"
#include "../Header/VulkanDescriptorSets.h"
#include "../Header/VulkanSemaphore.h"

using namespace LKEngine::Vulkan;

VulkanDevice::VulkanDevice(LKEngine::Window::WindowsWindow* window)
	:window(window),
	instance(nullptr),
	vkDevice(VK_NULL_HANDLE),
	gpu(VK_NULL_HANDLE),
	surface(VK_NULL_HANDLE),
	swapchain(nullptr),
	graphicsQueue(nullptr),
	presentQueue(nullptr),
	renderPass(nullptr),
	commandPool(nullptr)
{
	instance = new VulkanInstance();
	swapchain = new VulkanSwapchain(this, window);
	renderPass = new VulkanRenderPass(this);
	commandPool = new VulkanCommandPool(this);
	graphicsPipeline = new VulkanGraphicsPipeline(this);
	descriptorSetLayout = new VulkanDescriptorSetLayout(this);
	imageAvailableSemaphore = new VulkanSemaphore(this);
	renderFinishedSemaphore = new VulkanSemaphore(this);
}

VulkanDevice::~VulkanDevice()
{
	SAFE_DELETE(instance);
	SAFE_DELETE(swapchain);
	SAFE_DELETE(renderPass);
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
	SAFE_DELETE(commandPool);
	SAFE_DELETE(graphicsPipeline);
	SAFE_DELETE(descriptorSetLayout);
	SAFE_DELETE(imageAvailableSemaphore);
	SAFE_DELETE(renderFinishedSemaphore);
}

void VulkanDevice::Init(bool debug)
{
	instance->Init(debug);

	CreateSurface(window);

	RequirePhysicalDevice();

	CreateDevice(debug);

	CreateQueue();

	swapchain->Init(gpu, surface, queueIndices);

	renderPass->Init(swapchain);

	swapchain->CreateFrameBuffers(renderPass);

	CreateDescriptorSetLayout();

	graphicsPipeline->Init(renderPass, swapchain, descriptorSetLayout);

	CreateCommandPool();

	CreateSemaphore();
}

void VulkanDevice::Shutdown()
{
	vkDeviceWaitIdle(vkDevice);

	imageAvailableSemaphore->Shutdown();
	renderFinishedSemaphore->Shutdown();

	commandPool->Shutdown();

	graphicsPipeline->Shutdown();

	renderPass->Shutdown();

	swapchain->Shutdown();

	vkDestroySurfaceKHR(instance->GetHandle(), surface, nullptr);

	instance->Shutdown();
}

void VulkanDevice::Draw()
{
	vkQueueWaitIdle(presentQueue->GetHandle());

	uint32_t imageIndex = 0;
	VkResult result = vkAcquireNextImageKHR(vkDevice, swapchain->GetHandle(), 
		std::numeric_limits<uint64_t>::max(),
		imageAvailableSemaphore->GetHandle(), 
		VK_NULL_HANDLE,
		&imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		ResizeWindow();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("스왑 체인 이미지 요청 실패");
	}

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	graphicsQueue->Submit(imageAvailableSemaphore, renderFinishedSemaphore, waitStages, commandPool->GetBuffer(imageIndex));
	presentQueue->Present(swapchain, renderFinishedSemaphore, imageIndex);
}

void VulkanDevice::ResizeWindow()
{
	Console_Log("리사이징 윈도우 시작");
	vkDeviceWaitIdle(vkDevice);

	//스왑 체인 재 생성
	{
		VulkanSwapchain* newSwapchain = new VulkanSwapchain(this, window);
		newSwapchain->Init(gpu, surface, queueIndices, swapchain);

		swapchain->Shutdown();
		SAFE_DELETE(swapchain);
	
		swapchain = newSwapchain;
	}
	//렌더 패스 재 생성
	{
		renderPass->Shutdown();
		SAFE_DELETE(renderPass);
		renderPass = new VulkanRenderPass(this);
		renderPass->Init(swapchain);
	}
	//그래픽 파이프라인 재 생성
	{
		graphicsPipeline->Shutdown();
		SAFE_DELETE(graphicsPipeline);
		graphicsPipeline = new VulkanGraphicsPipeline(this);
		graphicsPipeline->Init(renderPass, swapchain, descriptorSetLayout);
	}
	//프레임 버퍼 재 생성
	{
		swapchain->CreateFrameBuffers(renderPass);
	}
	//커맨드 버퍼 재 생성
	{
		commandPool->FreeBuffers();
		commandPool->AllocBuffers(swapchain); 
		std::vector<VkClearValue> clearValues(2);
		clearValues[0].color = { 0.0f, 0.0f, 1.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		commandPool->Record(swapchain, renderPass, graphicsPipeline, clearValues);
	}
	Console_Log("리사이징 윈도우 성공");
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

uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(gpu, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

QueueFamilyIndices VulkanDevice::GetQueueFamilyIndices() const
{
	return queueIndices;
}

VkDevice VulkanDevice::GetHandle() const
{
	return vkDevice;
}

void VulkanDevice::RequirePhysicalDevice()
{
	Console_Log("그래픽 카드 선택");
	gpu = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance->GetHandle(), &deviceCount, devices.data());

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

	queueIndices.FindQueueFamily(gpu, surface);

	std::set<int> uniqueQueueFamilies = { queueIndices.graphicsFamily,queueIndices.presentFamily };
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

	Console_Log("디바이스 생성 성공");
}

void VulkanDevice::CreateQueue()
{
	Console_Log("Queue 생성 시작");
	graphicsQueue = new VulkanQueue(this, queueIndices.graphicsFamily, 0);
	presentQueue = new VulkanQueue(this, queueIndices.presentFamily, 0);
	Console_Log("Queue 생성 성공");
}

void VulkanDevice::CreateDescriptorSetLayout()
{
	Console_Log("DescriptorSetLayout 생성 시작");
	descriptorSetLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0);
	descriptorSetLayout->AddDescriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1);
	descriptorSetLayout->CreateDescriptorSetLayout();
	Console_Log("DescriptorSetLayout 생성 성공");
}

void VulkanDevice::CreateCommandPool()
{
	commandPool->Init(0, graphicsQueue->GetFamilyIndex());
	commandPool->AllocBuffers(swapchain);

	std::vector<VkClearValue> clearValues(2);
	clearValues[0].color = { 0.0f, 0.0f, 1.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	commandPool->Record(swapchain, renderPass, graphicsPipeline, clearValues);
}

void VulkanDevice::CreateSemaphore()
{
	Console_Log("세마포어 생성 시작");
	imageAvailableSemaphore->Init();
	renderFinishedSemaphore->Init();
	Console_Log("세마포어 생성 성공");
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
	Console_Log("Surface 생성 시작");
	VkResult result = glfwCreateWindowSurface(instance->GetHandle(), window->GetWindowHandle(), nullptr, &surface);
	Check_Throw(result != VK_SUCCESS, "Surface가 생성되지 않았습니다!");
	Console_Log("Surface 생성 성공");
}
