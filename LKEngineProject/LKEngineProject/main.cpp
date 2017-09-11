#include <stdlib.h>

#include "Application.h"

int main()
{
	using namespace LKEngine::Application;

	Application app(800,600);

	app.Loop();

	return 0;
}