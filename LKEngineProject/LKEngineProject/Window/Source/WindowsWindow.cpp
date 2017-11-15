#include "../Header/WindowsWindow.h"

#include "../../Utility/Header/Macro.h"

using namespace LKEngine::Window;

WindowsWindow::WindowsWindow(const int width,const int height)
	: 
	IWindow(width,height),
	window(nullptr)
{ }

WindowsWindow::~WindowsWindow()
{ }

bool WindowsWindow::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

void WindowsWindow::PollEvents()
{
	glfwPollEvents();
}

void WindowsWindow::Init()
{
	Console_Log("창 생성 시작");

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	Check_Throw(window == nullptr, "창 생성 실패!");
	Console_Log_If(window, "창 생성 성공");
}

void WindowsWindow::Shutdown()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow * WindowsWindow::GetWindowHandle() const
{
	return window;
}
