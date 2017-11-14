#include "../Header/Window.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Window;

Window::Window(const int width,const int height)
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

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow * LKEngine::Window::Window::GetWindowHandle() const
{
	return window;
}
