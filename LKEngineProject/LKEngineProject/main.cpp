#include "Application/Header/Application.h"

#include <cstdlib>
#include <crtdbg.h>

int main()
{
	//_CrtSetBreakAlloc(171);

	LKEngine::Application::Application app(800, 600);

	app.Loop();

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	return 0;
}