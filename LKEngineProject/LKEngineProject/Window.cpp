#include "Window.h"

#include "Macro.h"

using namespace LKEngine;

LKEngine::Window::Window::Window(const int width,const int height)
	:
	window(nullptr),
	width(width),
	height(height)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	Check_Throw(window == nullptr, "芒 积己 角菩!");
	Console_Log_If(window, "芒 积己 己傍");
}

LKEngine::Window::Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool LKEngine::Window::Window::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void LKEngine::Window::Window::PollEvents()
{
	glfwPollEvents();
}
