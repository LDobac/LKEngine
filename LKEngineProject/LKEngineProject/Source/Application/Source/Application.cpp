#include "../Header/Application.h"

#include "../../Utility/Header/Macro.h"

#include "../../Window/Header/WindowsWindow.h"
#include "../../Renderer/Header/VulkanDevice.h"

#include "../../Application/Header/PipelineManager.h"
#include "../../Application/Header/SceneManager.h"
#include "../../Application/Header/EntityPool.h"
#include "../../Application/Header/AssetManager.h"
#include "../../Application/Header/Time.h"

USING_LK_SPACE;

Application::Application()
	:isExit(false)
{ }

Application::~Application()
{
	device->WaitIdle();

	SceneManager::Release();
	EntityPool::Release();
	PipelineManager::Release();
	AssetManager::Release();

	Vulkan::VulkanDevice::Release();
	window->Shutdown();

	SAFE_DELETE(window);
}

void LKEngine::Application::Application::Start(int width, int height, LKEngine::Scene* startScene, bool debugMode)
{
	window = new Window::WindowsWindow(width, height);
	device = Vulkan::VulkanDevice::GetInstance();

	device->SetDebugMode(debugMode);
	device->SetWindow(window);

	window->Init(device);
	device->Init();

	LKEngine::Time::Start();

	SceneManager::GetInstance()->Start(startScene);
}

void LKEngine::Application::Application::Exit()
{
	isExit = true;
}

void LKEngine::Application::Application::Loop()
{
	while (!window->WindowShouldClose() && !isExit)
	{
		LKEngine::Time::Update();

		SceneManager::GetInstance()->Update();

		device->Render();
		device->Draw();

		window->PollEvents();
	}
}
