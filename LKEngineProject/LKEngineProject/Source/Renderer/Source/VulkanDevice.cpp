#include "../Header/VulkanDevice.h"

#include <vector>
#include <set>
#include <numeric>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Header/VulkanQueue.h"
#include "../Header/VulkanExtension.h"
#include "../Header/VulkanSwapchain.h"
#include "../Header/VulkanRenderPass.h"
#include "../Header/VulkanCommandPool.h"
#include "../Header/VulkanSemaphore.h"

#include "../../Application/Header/PipelineManager.h"

#include "../../Application/Header/EntityPool.h"
#include "../../Application/Header/Time.h"
#include "../../Utility/Header/Macro.h"
#include "../../Window/Header/WindowsWindow.h"

USING_LK_VULKAN_SPACE

VulkanDevice::VulkanDevice()
	: isDebug(false),
	window(nullptr),
	vulkanInstance(nullptr),
	gpu(VK_NULL_HANDLE),
	vkDevice(VK_NULL_HANDLE),
	surface(VK_NULL_HANDLE),
	swapchain(nullptr),
	graphicsQueue(nullptr),
	presentQueue(nullptr),
	renderPass(nullptr),
	commandPool(nullptr),
	singleCommandPool(nullptr),
	imageAvailableSemaphore(nullptr),
	renderFinishedSemaphore(nullptr)
{ }

VulkanDevice::~VulkanDevice()
{
	SAFE_DELETE(vulkanInstance);
	SAFE_DELETE(swapchain);
	SAFE_DELETE(renderPass);
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
	SAFE_DELETE(commandPool);
	SAFE_DELETE(singleCommandPool);
	SAFE_DELETE(imageAvailableSemaphore);
	SAFE_DELETE(renderFinishedSemaphore);
}

VulkanDevice* VulkanDevice::instance = nullptr;
VulkanDevice * VulkanDevice::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new VulkanDevice();
	}

	return instance;
}

void VulkanDevice::Release()
{
	instance->Shutdown();
	SAFE_DELETE(instance);
}

void VulkanDevice::SetDebugMode(bool debug)
{
	isDebug = debug;
}

void VulkanDevice::SetWindow(LKEngine::Window::WindowsWindow * window)
{
	this->window = window;
}

void VulkanDevice::Init()
{
	vulkanInstance = new VulkanInstance();
	swapchain = new VulkanSwapchain(this, window);
	renderPass = new VulkanRenderPass(this);
	commandPool = new VulkanCommandPool(this);
	singleCommandPool = new VulkanSingleCommandPool(this);
	imageAvailableSemaphore = new VulkanSemaphore(this);
	renderFinishedSemaphore = new VulkanSemaphore(this);

	InitInstance();
	InitSurface();
	RequireGPU();
	InitDevice();
	CreateQueue();
	InitSwapchain();
	InitCommandPool();
	AllocCommandBuffers();
	InitDepthBuffer();
	InitRenderPass();
	CreateFramebuffers();
	CreateSemaphore();
}

void VulkanDevice::Shutdown()
{
	WaitIdle();
	
	imageAvailableSemaphore->Shutdown();
	renderFinishedSemaphore->Shutdown();

	renderPass->Shutdown();

	commandPool->Shutdown();
	singleCommandPool->Shutdown();

	swapchain->Shutdown();

	vkDestroySurfaceKHR(vulkanInstance->GetHandle(), surface, nullptr);

	vulkanInstance->Shutdown();
}

void VulkanDevice::Render()
{
	std::vector<VkClearValue> clearValues(2);
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	std::vector<VkFramebuffer> framebuffers = swapchain->GetFrameBuffers();
	VkExtent2D extent = swapchain->GetExtent();

	for (size_t i = 0; i < commandPool->GetBufferSize(); i++)
	{
		auto commandBuffer = commandPool->GetBuffer(i);

		commandPool->RecordBegin(i, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
		renderPass->Begin(clearValues, framebuffers[i], extent, commandBuffer);

		EntityPool::GetInstance()->Render(commandBuffer);

		renderPass->End(commandBuffer);
		commandPool->RecordEnd(i);
	}
}

void VulkanDevice::Draw()
{
	if (!EntityPool::GetInstance()->NeedRender())
	{
		presentQueue->WaitIdle();

		uint32_t imageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(vkDevice, swapchain->GetHandle(),
			std::numeric_limits<uint64_t>::max(),
			imageAvailableSemaphore->GetHandle(),
			VK_NULL_HANDLE,
			&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			Console_Log("스왑체인 데이터가 만료됨, 다시 렌더");
			ResizeWindow();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			Console_Log("스왑체인 이미지 가져오기 실패");
			throw std::runtime_error("");
		}

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		graphicsQueue->Submit(imageAvailableSemaphore, renderFinishedSemaphore, waitStages, commandPool->GetBuffer(imageIndex));
		presentQueue->Present(swapchain, renderFinishedSemaphore, imageIndex);
	}
}

void VulkanDevice::ResizeWindow()
{
	WaitIdle();

	VulkanSwapchain* newSwapchain = new VulkanSwapchain(this, window);
	newSwapchain->Init(gpu, surface, queueIndices, swapchain);
	newSwapchain->InitDepthBuffer(singleCommandPool);
	swapchain->Shutdown();
	SAFE_DELETE(swapchain);
	swapchain = newSwapchain;

	renderPass->Shutdown();
	SAFE_DELETE(renderPass);
	renderPass = new VulkanRenderPass(this);
	renderPass->Init(swapchain);

	swapchain->InitDepthBuffer(singleCommandPool);
	swapchain->CreateFrameBuffers(renderPass);

	commandPool->FreeBuffers();
	commandPool->AllocBuffers(swapchain->GetFrameBuffers().size());

	PipelineManager::GetInstance()->RecreatePipelines();

	Render();
}

void VulkanDevice::WaitIdle()
{
	vkDeviceWaitIdle(vkDevice);
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

	Check_Throw(true, "포맷 찾기 실패");
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

VulkanSwapchain * VulkanDevice::GetSwapchain() const
{
	return swapchain;
}

VulkanSingleCommandPool * VulkanDevice::GetSingleCommandPool() const
{
	return singleCommandPool;
}

VulkanRenderPass * VulkanDevice::GetRenderPass() const
{
	return renderPass;
}

void VulkanDevice::InitInstance()
{
	vulkanInstance->Init(isDebug);
}

void VulkanDevice::InitSurface()
{
	VkResult result = glfwCreateWindowSurface(vulkanInstance->GetHandle(), window->GetWindowHandle(), nullptr, &surface);
	Check_Throw(result != VK_SUCCESS, "Surface 생성 실패");
}

void VulkanDevice::RequireGPU()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance->GetHandle(), &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(vulkanInstance->GetHandle(), &deviceCount, devices.data());

	//TODO : GPU가 여러개 일경우 그중 가장 좋은 GPU 고르기
	for (auto device : devices)
	{
		if (CheckDeviceFeatures(device))
		{
			gpu = device;
			break;
		}
	}

	Check_Throw(gpu == VK_NULL_HANDLE, "GPU 탐색 실패");

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

	Console_Log("GPU 정보 : ");
	Console_Log_Format("API : 0x%x , Driver : 0x%x , VecdorID : 0x%x", gpuProp.apiVersion, gpuProp.driverVersion, gpuProp.vendorID);
	Console_Log_Format("Name \"%s\" , DeviceID : 0x%x , Type \"%s\"", gpuProp.deviceName, gpuProp.deviceID, GetDeviceTypeString().c_str());
	Console_Log_Format("Max Descriptor Sets Bound : %d , Timestamps : %d", gpuProp.limits.maxBoundDescriptorSets, gpuProp.limits.timestampComputeAndGraphics);
}

void VulkanDevice::InitDevice()
{
	queueIndices.FindQueueFamily(gpu, surface);

	std::set<int> uniqueQueueFamilies = { queueIndices.graphicsFamily,queueIndices.presentFamily };
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	//TODO : Queue가 여러개시 우선순위로 큐 지정
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

	//TODO : GPU 기능 더 확인하기
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(Vulkan::deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = Vulkan::deviceExtensions.data();

	if (isDebug)
	{
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::vaildationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = Vulkan::vaildationLayers.data();
	}
	else
	{
		deviceCreateInfo.enabledLayerCount = 0;
	}

	Check_Throw(vkCreateDevice(gpu, &deviceCreateInfo, nullptr, &vkDevice) != VK_SUCCESS, "Vulkan Device 생성 실패");
}

void VulkanDevice::CreateQueue()
{
	graphicsQueue = new VulkanQueue(this, queueIndices.graphicsFamily, 0);
	presentQueue = new VulkanQueue(this, queueIndices.presentFamily, 0);
}

void VulkanDevice::InitCommandPool()
{
	commandPool->Init(0, graphicsQueue->GetFamilyIndex());
	singleCommandPool->Init(graphicsQueue);
}

void VulkanDevice::AllocCommandBuffers()
{
	commandPool->AllocBuffers(swapchain->GetFrameBuffers().size());
}

void VulkanDevice::InitSwapchain()
{
	swapchain->Init(gpu, surface, queueIndices);
}

void VulkanDevice::InitDepthBuffer()
{
	swapchain->InitDepthBuffer(singleCommandPool);
}

void VulkanDevice::InitRenderPass()
{
	renderPass->Init(swapchain);
}

void VulkanDevice::CreateFramebuffers()
{
	swapchain->CreateFrameBuffers(renderPass);
}

void VulkanDevice::CreateSemaphore()
{
	imageAvailableSemaphore->Init();
	renderFinishedSemaphore->Init();
}

bool VulkanDevice::CheckDeviceFeatures(VkPhysicalDevice device)
{
	QueueFamilyIndices queueFamilyIndices;
	VulkanExtension extension;
	SwapchainSupportDetail supportDetail(device, surface);

	if (queueFamilyIndices.FindQueueFamily(device, surface))
	{
		bool deviceExtensionSupport = extension.CheckDeviceExtensionSupport(device);
		bool swapSupport = supportDetail.CheckSwapchainAdequate();

		//TODO : GPU가 지원하는 기능 더 확인하기
		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
		bool deviceFeatureSupport = supportedFeatures.samplerAnisotropy;

		return deviceExtensionSupport && deviceFeatureSupport && swapSupport;
	}

	return false;
}