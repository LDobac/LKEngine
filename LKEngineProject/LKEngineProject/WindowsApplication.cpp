#include "WindowsApplication.h"

#include "Macro.h"

using namespace LKEngine;

void Window::WindowsApplication::Init()
{
	glfwInit();	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	vkDevice.Init();
}

void Window::WindowsApplication::CreateWindow(const int width, const int height)
{
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

	vkDevice.CleanUp();
}
