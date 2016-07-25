#include "Application.h"

void Application::run()
{
	mainLoop();
}

void Application::mainLoop()
{
	while (!graphics.windowClosed())
	{
		glfwPollEvents();
		graphics.Draw();
	}

}

