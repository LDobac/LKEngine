#include "../Header/WindowsWindow.h"

#include "../../Utility/Header/Macro.h"
#include "../../Vulkan/Header/VulkanDevice.h"

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

void WindowsWindow::Init(LKEngine::Vulkan::VulkanDevice* device)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);

	glfwSetWindowUserPointer(window, device);
	glfwSetWindowSizeCallback(window, WindowsWindow::onWindowResized);

	Check_Throw(window == nullptr, "창 생성 실패!");
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

void WindowsWindow::onWindowResized(GLFWwindow * window, int width, int height)
{
	if (width == 0 || height == 0) return;

	auto device = reinterpret_cast<LKEngine::Vulkan::VulkanDevice*>(glfwGetWindowUserPointer(window));
	device->ResizeWindow();
}
