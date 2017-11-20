#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Vulkan/Header/VulkanDevice.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = new Vulkan::VulkanDevice(window);

	window->Init(device);
	device->Init(true);
}

Application::~Application()
{
	device->WaitIdle();

	device->Shutdown();
	window->Shutdown();

	SAFE_DELETE(window);
	SAFE_DELETE(device);
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
