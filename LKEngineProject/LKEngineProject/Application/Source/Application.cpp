#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Vulkan/Header/VulkanDevice.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	Console_Log("초기화 시작");

	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = new Vulkan::VulkanDevice(window);

	window->Init(device);
	device->Init(true);

	Console_Log("초기화 성공");
}

Application::~Application()
{
	device->Shutdown();
	window->Shutdown();

	SAFE_DELETE(window);
	SAFE_DELETE(device);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		device->Draw();
		window->PollEvents();
	}
}
