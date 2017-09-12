#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/Window.h"
#include "../../Vulkan/Header/VulkanDevice.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	window = new Window::Window(windowWidth,windowHeight);
	device = new Vulkan::VulkanDevice();

	device->Init(window,true);
}


Application::~Application()
{
	device->Shutdown();

	SAFE_DELETE(window);
	SAFE_DELETE(device);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		window->PollEvents();
	}
}
