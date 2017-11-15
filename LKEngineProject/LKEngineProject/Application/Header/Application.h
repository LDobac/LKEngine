#pragma once

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice;
	}

	namespace Window
	{
		class WindowsWindow;
	}
}

namespace LKEngine::Application
{
	class Application
	{
	private:
		Window::WindowsWindow* window;
		Vulkan::VulkanDevice* device;

	public:
		Application(const int windowWidth,const int windowHeight);
		~Application();

		void Loop();
	};
}

