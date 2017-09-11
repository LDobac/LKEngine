#include "Application.h"

#include "Macro.h"

#include "Window.h"
#include "VulkanDevice.h"

using namespace LKEngine::Application;

Application::Application(const int windowWidth, const int windowHeight)
{
	window = new Window::Window(windowWidth,windowHeight);
	device = new Vulkan::VulkanDevice();
}


Application::~Application()
{
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
