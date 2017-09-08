#include "WindowsApplication.h"

#include "Macro.h"

using namespace LKEngine;

LKEngine::Window::WindowsApplication::WindowsApplication()
{
	vkDevice = new Vulkan::VulkanDevice();
}

LKEngine::Window::WindowsApplication::~WindowsApplication()
{
	delete vkDevice;
}

void Window::WindowsApplication::CreateWindow(const int width, const int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
}

void LKEngine::Window::WindowsApplication::MainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void Window::WindowsApplication::CleanUp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
