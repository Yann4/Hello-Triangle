#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Graphics.h"

class Application
{
public:
	void run();

private:
	bool shouldQuit();
	void mainLoop();

private:
	Graphics graphics;
	
};
