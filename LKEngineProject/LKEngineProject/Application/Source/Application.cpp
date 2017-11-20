#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Vulkan/Header/VulkanDevice.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = Vulkan::VulkanDevice::GetInstance();

	device->SetDebugMode(true);
	device->SetWindow(window);

	window->Init(device);
	device->Init();
}

Application::~Application()
{
	device->WaitIdle();

	Vulkan::VulkanDevice::Release();
	window->Shutdown();

	SAFE_DELETE(window);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		device->Update();
		device->Draw();
		window->PollEvents();
	}
}
