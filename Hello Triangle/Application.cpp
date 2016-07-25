#include "Application.h"

void Application::run()
{
	mainLoop();
}

void Application::mainLoop()
{
	while (!graphics.shouldExit())
	{
		glfwPollEvents();
		graphics.Draw();
	}

}

