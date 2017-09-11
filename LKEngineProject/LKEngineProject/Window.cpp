#include "Window.h"

#include "Macro.h"

using namespace LKEngine;

LKEngine::Window::Window::Window(const int width,const int height)
	:
	window(nullptr),
	width(width),
	height(height)
{
	Console_Log("창 생성 시작");

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	Check_Throw(window == nullptr, "창 생성 실패!");
	Console_Log_If(window, "창 생성 성공");
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
