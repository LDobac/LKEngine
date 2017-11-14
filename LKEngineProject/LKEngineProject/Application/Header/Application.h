#pragma once

namespace LKEngine
{
	namespace Vulkan
	{
		class VulkanDevice;
	}

	namespace Window
	{
		class Window;
	}

	namespace Application
	{
		class Application
		{
		private:
			Window::Window* window;
			Vulkan::VulkanDevice* device;

		public:
			Application(const int windowWidth,const int windowHeight);
			~Application();

			void Loop();
		};
	}
}

