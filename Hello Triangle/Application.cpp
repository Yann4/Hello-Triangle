#include "Application.h"

void Application::run()
{
	mainLoop();
}

void Application::mainLoop()
{
	while (!shouldQuit())
	{
		glfwPollEvents();
		graphics.Draw();
	}
}

bool Application::shouldQuit()
{
	return graphics.windowClosed();
}