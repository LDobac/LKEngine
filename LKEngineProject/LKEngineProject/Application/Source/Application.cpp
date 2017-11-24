#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Vulkan/Header/VulkanDevice.h"

#include "../../Src/PipelineManager.h"
#include "../../Src/SceneManager.h"
#include "../../Src/EntityPool.h"
#include "../../Src/Time.h"

using namespace LKEngine::Application;

#include "../../Source/TestScene.h"

Application::Application(const int windowWidth, const int windowHeight)
{
	Console_Log("초기화 시작");
	window = new Window::WindowsWindow(windowWidth,windowHeight);
	device = Vulkan::VulkanDevice::GetInstance();

	device->SetDebugMode(true);
	device->SetWindow(window);

	window->Init(device);
	device->Init();

	LKEngine::Time::Start();

	SceneManager::GetInstance()->Start(new TestScene());

	Console_Log("초기화 성공");

}

Application::~Application()
{
	device->WaitIdle();

	SceneManager::Release();
	EntityPool::Release();
	PipelineManager::Release();
	

	Vulkan::VulkanDevice::Release();
	window->Shutdown();

	SAFE_DELETE(window);
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose())
	{
		LKEngine::Time::Update();

		SceneManager::GetInstance()->Update();
		EntityPool::GetInstance()->Update();

		device->Render();

		device->Draw();

		window->PollEvents();
	}
}
